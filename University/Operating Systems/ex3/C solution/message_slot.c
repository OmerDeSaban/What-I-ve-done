/**
 * @file message_slot.c
 * @brief Kernel module implementation of message slot IPC mechanism.
 *
 * This module implements a character device driver for inter-process communication. Each device file
 * (identified by minor number) represents a message slot that can contain multiple message channels.
 * 
 * I've added the module metadata to try and do this as if this is an assignment given to an employee in
 * a high-tech company. I've learned about it while searching how to create a proper kernel module, which
 * led me to find this:
 * https://medium.com/dvt-engineering/how-to-write-your-first-linux-kernel-module-cf284408beeb
 * I didn't read much, but I did notice that they had metadata even in their first example, and I liked
 * the idea.
 * 
 * I learned about spinlock protection by doing the following search:
 * https://www.google.com/search?q=preventing+a+process+interruption&oq=preventing+a+process+interruption&gs_lcrp=EgZjaHJvbWUyBggAEEUYOTIHCAEQIRiPAtIBCDY1NDNqMGoxqAIAsAIA&sourceid=chrome&ie=UTF-8
 * I then read the relevant Wikipedia entry (well, the first few lines of the Wikipedia entry), and
 * looked up its header file. Found it here, on GitHub:
 * https://github.com/torvalds/linux/blob/master/include/linux/spinlock.h
 * In said header file I looked up the functions relating to spinlock protection, and found 2 that seemed
 * harmless enough for the project, yet useful enough to maybe be used for a larger-scale project.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include "message_slot.h"

/* Module metadata: */
MODULE_AUTHOR("Omer David De-Saban");
MODULE_DESCRIPTION("A simple message slot IPC driver");
MODULE_LICENSE("GPL");

/* Channel structure for each message channel in a slot */
typedef struct message_channel {
    unsigned int channel_id;        /* This channel's ID */
    char* message;                  /* Current message's content */
    size_t msg_len;                 /* Current message's length */
    struct message_channel* next;   /* Next channel in linked list */
} message_channel;

/* Slot structure for each minor device number */
typedef struct message_slot {
    message_channel* channels;      /* Linked list of channels */
    unsigned int minor;             /* This slot's minor number */
    spinlock_t lock;                /* Protection for concurrent access */
} message_slot;

/* Array of all message slots, indexed by minor number */
static message_slot *message_slots[256] = {NULL};

/**
 * @brief Find or create a channel in a slot.
 * @param slot A pointer to a message_slot.
 * @param channel_id The channel ID to find/create.
 * @return A pointer to a message_channel, or NULL on error.
 */
static message_channel* get_channel(message_slot *slot, unsigned int channel_id) {
    message_channel *curr, *new_channel;
    unsigned long flags;

    spin_lock_irqsave(&slot->lock, flags);                  /* Save interrupt state and lock */

    /* Search for existing channel */
    curr = slot->channels;
    while (curr != NULL) {
        if (curr->channel_id == channel_id) {               /* Found the right channel */
            spin_unlock_irqrestore(&slot->lock, flags);     /* Restore interrupt state and unlock */
            return curr;
        }
        curr = curr->next;
    }

    /* Create new channel if not found */
    new_channel = kmalloc(sizeof(message_channel), GFP_KERNEL);
    if (new_channel == NULL) {                              /* Allocation error */
        spin_unlock_irqrestore(&slot->lock, flags);         /* Restore interrupt state and unlock */
        return NULL;
    }

    /* Initialize new channel */
    new_channel->channel_id = channel_id;
    new_channel->message = NULL;
    new_channel->msg_len = 0;
    new_channel->next = slot->channels;
    slot->channels = new_channel;

    spin_unlock_irqrestore(&slot->lock, flags);             /* Restore interrupt state and unlock */
    return new_channel;
}

/**
 * @brief Open a message slot device
 * @param node A pointer to the inode that contains the file's major and minor numbers.
 * @param filename A pointer to a a kernel file structure that represents an open instance of the device.
 * @return SUCCESS, or -ENOMEM on error.
 */
static int device_open(struct inode* node, struct file* filename) {
    unsigned int minor;
    message_slot *slot;

    /* Following the suggestion from msgslot.pdf's section 3.1.3: */
    minor = iminor(node);

    /* Create slot if it doesn't exist */
    if (message_slots[minor] == NULL) {
        slot = kmalloc(sizeof(message_slot), GFP_KERNEL);
        if (slot == NULL) {                                 /* Allocation error */
            printk(KERN_ERR "%s: Failed to allocate slot for minor %u.\n", KERNEL_MODULE_NAME, minor);
            return -ENOMEM;
        }

        slot->channels = NULL;
        slot->minor = minor;
        spin_lock_init(&slot->lock);
        message_slots[minor] = slot;
    }

    filename->private_data = NULL;  /* No channel selected initially */
    return SUCCESS;
}

/**
 * @brief Release a message slot device.
 * @param node A pointer to the inode that contains the file's major and minor numbers.
 * @param filename A pointer to a a kernel file structure that represents an open instance of the device.
 * @return Always returns SUCCESS.
 */
static int device_release(struct inode* node, struct file* filename) {
    printk(KERN_DEBUG "%s: Device with minor %u has been released.\n", KERNEL_MODULE_NAME, iminor(node));
    return SUCCESS;
}

/**
 * @brief Handle ioctl commands for message_slot.
 * @param filename A pointer to a a kernel file structure that represents an open instance of the device.
 * @param ioctl_command_id The IOCTL command's ID.
 * @param ioctl_param The IOCTL data.
 * @return SUCCESS, or -EINVAL on error.
 */
static long device_ioctl(struct file* filename, unsigned int ioctl_command_id,  unsigned long ioctl_param) {
    /* Validate command and parameter: */
    if (ioctl_command_id != MSG_SLOT_CHANNEL || ioctl_param == CHANNEL_UNSET) { return -EINVAL; }

    filename->private_data = (void*) ioctl_param;
    return SUCCESS;
}

/**
 * @brief Writes a message to a channel.
 * @param filename A pointer to a a kernel file structure that represents an open instance of the device.
 * @param buffer A user space buffer, containing data to write.
 * @param length The length of the data to write.
 * @return The length of the written data. On error:
 * 1. -EINVAL if the parameters don't meet the requirements set in msgslot.pdf, or if the relevant slot
 * doesn't exist.
 * 2. -ENOMEM if the relevant channel doesn't exist, or if there was an allocation error.
 * 3. -EFAULT if user's message failed to copy.
 */
static ssize_t device_write(struct file *filename, const char __user *buffer, size_t length, loff_t *offset) {
    message_slot *slot;
    message_channel *channel;
    char *temp_buf;
    unsigned int minor;
    unsigned long channel_id;

    /* Parameters validation: */
    if (filename->private_data == NULL || length == 0 || length > MAXIMAL_BUFFER) { return -EINVAL; }

    /* Following the suggestion from msgslot.pdf's section 3.1.3: */
    minor = iminor(filename->f_inode);

    channel_id = (unsigned long) filename->private_data;
    slot = message_slots[minor];

    if (slot == NULL) { return -EINVAL; }           /* No such slot exists */

    /* Get or create channel */
    channel = get_channel(slot, channel_id);
    if (channel == NULL) { return -ENOMEM; }        /* No such channel exists */

    /* Allocate buffer for new message */
    temp_buf = kmalloc(length, GFP_KERNEL);
    if (temp_buf == NULL) { return -ENOMEM; }       /* Allocation error */

    /* Copy message from user space */
    if (copy_from_user(temp_buf, buffer, length) != 0) {
        kfree(temp_buf);
        return -EFAULT;
    }

    /* Update channel with new message */
    if (channel->message != NULL) { kfree(channel->message); }
    channel->message = temp_buf;
    channel->msg_len = length;

    return length;
}

/**
 * @brief Reads a message from a channel.
 * @param filename A pointer to a a kernel file structure that represents an open instance of the device.
 * @param buffer A user space buffer, containing data to read.
 * @param length The length of the data to read.
 * @return The length of the data read. On error:
 * 1. -EINVAL if the paramets don't meet the requirements set in msgslot.pdf, or the relevant slot
 * doesn't exist.
 * 2. -EWOULDBLOCK if the relevant channel doesn't exit, or if the relevant message doesn't exit.
 * 3. -ENOSPC if the buffer is too small.
 * 4. -EFAULT if the user's message failed to copy.
 */
static ssize_t device_read(struct file *filename, char __user *buffer, size_t length, loff_t *offset) {
    message_slot *slot;
    message_channel *channel;
    unsigned int minor;
    unsigned long channel_id;

    /* Parameters validation: */
    if (filename->private_data == NULL) { return -EINVAL; }

    /* Following the suggestion from msgslot.pdf's section 3.1.3: */
    minor = iminor(filename->f_inode);

    channel_id = (unsigned long) filename->private_data;
    slot = message_slots[minor];

    if (slot == NULL) { return -EINVAL; }                   /* No such slot exists */

    /* Get channel */
    channel = get_channel(slot, channel_id);
    if (channel == NULL || channel->message == NULL) {      /* No such channel exists, or no message exists */
        return -EWOULDBLOCK; }

    /* Check buffer size */
    if (length < channel->msg_len) { return -ENOSPC; }

    /* Copy message to user space */
    if (copy_to_user(buffer, channel->message, channel->msg_len) != 0) { return -EFAULT; }

    return channel->msg_len;
}

/* File operations structure */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl,
};

/**
 * @brief Initializes the message_slot module.
 * Takes no arguments.
 * @return SUCCESS if the module loaded successfully, or the character device driver whose registration
 * failed on error.
 */
static int __init message_slot_init(void) {
    int rc;

    rc = register_chrdev(MAJOR_NUM, KERNEL_MODULE_NAME, &fops);

    if (rc < 0) {                                               /* Registration failure */
        printk(KERN_ERR "%s: Registration failed for %d.\n", KERNEL_MODULE_NAME, MAJOR_NUM);
        return rc;
    }

    printk(KERN_INFO "%s: Module loaded successfully.\n", KERNEL_MODULE_NAME);
    return SUCCESS;
}

/**
 * @brief Does the cleanup of the message_slot module.
 * Takes no arguments.
 */
static void __exit message_slot_cleanup(void) {
    int i;
    message_slot *slot;
    message_channel *curr, *next;

    /* Frees all allocated memory: */
    for (i = 0; i < 256; i++) {
        slot = message_slots[i];
        if (slot != NULL) {
            curr = slot->channels;
            while (curr != NULL) {
                next = curr->next;
                if (curr->message != NULL) { kfree(curr->message); }
                kfree(curr);
                curr = next;
            }
            kfree(slot);
            message_slots[i] = NULL;
        }
    }

    unregister_chrdev(MAJOR_NUM, KERNEL_MODULE_NAME);
    printk(KERN_INFO "%s: Module unloaded successfully\n", KERNEL_MODULE_NAME);
}

module_init(message_slot_init);
module_exit(message_slot_cleanup);
