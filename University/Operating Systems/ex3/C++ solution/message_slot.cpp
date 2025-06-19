/**
 * @file message_slot.cpp
 * @brief The C++ version of the required message_slot.c file.
 * 
 * Yes, I know a Linux kernel and C++ don't mix well, but I just wanted to practice some C++, as I
 * already did this assignment in C in the past.
 * Please regard this as a "if you translate this to C, this would be my implementation".
 */

#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h> 
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/types.h>
#include <cstddef>

MODULE_LICENSE("GPL");

#include "message_slot.h"

/**A struct that stores a single message.
 * Represents a single node in the linked list of messages.
 */
struct Channel {
    unsigned int id;
    char msg[128];
    size_t msg_length;
    Channel* next;
};

/**A struct that stores the data of a single message slot (= device file).
 * Represents a single node in the linked list of message slots.
 */
struct DeviceFile {
    int minor;
    Channel* channels;
    DeviceFile* next;
};

static DeviceFile* device_list_head = nullptr;

static int device_open(struct inode* inode, struct file* file) {
    int minor = iminor(inode);
    DeviceFile* curr = device_list_head;

    while (curr != nullptr) {
        if (curr->minor == minor) { return 0; }
        curr = curr->next;
    }

    DeviceFile* new_device = (DeviceFile*)kmalloc(sizeof(DeviceFile), GFP_KERNEL);
    if (new_device == nullptr) {
        printk(KERN_ERR "message_slot: failed to allocate memory for a new device!\n");
        return -ENOMEM;
    }

    new_device->minor = minor;
    new_device->channels = nullptr;
    new_device->next = device_list_head;
    device_list_head = new_device;

    return 0;
}

static long device_ioctl(struct file* file, unsigned int ioctl_command_id, unsigned long ioctl_param) {
    if (ioctl_command_id != MSG_SLOT_CHANNEL) {
        printk(KERN_ERR "message_slot: The passed command is not MSG_SLOT_CHANNEL!\n");
        return -EINVAL;
    }

    if (ioctl_param <= 0) {
        printk(KERN_ERR "message_slot: The passed channel id isn't positive!\n");
        return -EINVAL;
    }

    file->private_data = (void*)(uintptr_t)ioctl_param;

    return 0;
}

int writeSetup(struct file* file, Channel** out_channel) {
    if ((uintptr_t)(file->private_data) == 0) {
        printk(KERN_ERR "message_slot: channel not set on file descriptor!\n");
        return -EINVAL;
    }

    unsigned int channel_id = (uintptr_t)(file->private_data);
    int minor = iminor(file->f_inode);
    
    DeviceFile* curr_device = device_list_head;
    while (curr_device != nullptr && curr_device->minor != minor) { curr_device = curr_device->next; }

    if (!curr_device) {
        printk(KERN_ERR "message_slot: device file not found for minor %d!\n", minor);
        return -EINVAL;
    }

    Channel* curr_channel = curr_device->channels;
    while (curr_channel != nullptr) {
        if (curr_channel->id == channel_id) {
            *out_channel = curr_channel;
            return 0;
        }
        curr_channel = curr_channel->next;
    }

    Channel* new_channel = (Channel*)kmalloc(sizeof(Channel), GFP_KERNEL);
    if (new_channel == nullptr) {
        printk(KERN_ERR "message_slot: failed to allocate memory for a new channel!\n");
        return -ENOMEM;
    }

    new_channel->id = channel_id;
    new_channel->msg_length = 0;
    new_channel->next = curr_device->channels;
    curr_device->channels = new_channel;

    *out_channel = new_channel;
    return 0;
}

static ssize_t device_write(struct file* file, const char __user* buffer, size_t length,
    loff_t* offset) {
    Channel* curr_channel;
    int setup_status = writeSetup(file, &curr_channel);
    if (setup_status != 0) { return setup_status; }
    
    if (length <= 0) {
        printk(KERN_ERR "message_slot: Passed message length is non-positive!\n");
        return -EMSGSIZE;
    }

    if (length > 128) {
        printk(KERN_ERR "message_slot: Passed message length is more than 128 bytes!\n");
        return -EMSGSIZE;
    }

    if (copy_from_user(curr_channel->msg, buffer, length) != 0) {
        printk(KERN_ERR "message_slot: Failed to copy message from user buffer!\n");
        return -EFAULT;
    }

    curr_channel->msg_length = length;
    return length;
}

int readSetup(struct file* file, Channel** out_channel) {
    if ((uintptr_t)(file->private_data) == 0) {
        printk(KERN_ERR "message_slot: channel not set on file descriptor!\n");
        return -EINVAL;
    }

    unsigned int channel_id = (uintptr_t)(file->private_data);
    int minor = iminor(file->f_inode);

    DeviceFile* curr_device = device_list_head;
    while (curr_device != nullptr && curr_device->minor != minor) { curr_device = curr_device->next; }

    if (!curr_device) {
        printk(KERN_ERR "message_slot: device file not found for minor %d!\n", minor);
        return -EINVAL;
    }

    Channel* curr_channel = curr_device->channels;
    while (curr_channel != nullptr && curr_channel->id != channel_id)
        { curr_channel = curr_channel->next; }

    if (curr_channel == nullptr) {
        printk(KERN_ERR "message_slot: channel %u not found for minor %d!\n", channel_id, minor);
        return -EWOULDBLOCK;
    }

    *out_channel = curr_channel;
    return 0;
}


static ssize_t device_read(struct file* file, char __user* buffer, size_t length, loff_t* offset) {
    Channel* curr_channel;
    int setup_status = readSetup(file, &curr_channel);
    if (setup_status != 0) { return setup_status; }
    unsigned int channel_id = curr_channel->id;

    if (curr_channel->msg_length == 0) {
        printk(KERN_ERR "message_slot: No message exists on channel %u!\n", channel_id);
        return -EWOULDBLOCK;
    }

    if (length < curr_channel->msg_length) {
        printk(KERN_ERR "message_slot: Provided buffer too small for message on channel %u!\n",
            channel_id);
        return -ENOSPC;
    }

    if (copy_to_user(buffer, curr_channel->msg, curr_channel->msg_length) != 0) {
        printk(KERN_ERR "message_slot: failed to copy message to the user buffer!\n");
        return -EFAULT;
    }

    return curr_channel->msg_length;
}

/**A struct denoting which functions should be used for each file operation */
struct file_operations Fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl,
};

static int __init message_slot_init(void) {
    int device_register_status = register_chrdev(MAJOR_NUM, "message_slot", &Fops);

    if (device_register_status < 0) {
        printk(KERN_ERR "message_slot: Failed to register the device!\n");
        return device_register_status;
    }
    
    return 0;
}

static void __exit message_slot_cleanup(void) {
    unregister_chrdev(MAJOR_NUM, "message_slot");

    DeviceFile* curr_device = device_list_head;
    while (curr_device != nullptr) {
        Channel* curr_channel = curr_device->channels;
        while (curr_channel != nullptr) {
            Channel* tmp_channel = curr_channel;
            curr_channel = curr_channel->next;
            kfree(tmp_channel);
        }
        DeviceFile* tmp_device = curr_device;
        curr_device = curr_device->next;
        kfree(tmp_device);
    }
}

module_init(message_slot_init);
module_exit(message_slot_cleanup);