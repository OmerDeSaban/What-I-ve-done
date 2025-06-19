/**
 * @file message_slot.h
 * @brief Header file for message slot IPC mechanism.
 * 
 * Contains definitions and declarations used throughout the message_slot project, including the kernel
 * module and the user space programs.
 * 
 * The message slot supports a single ioctl command (MSG_SLOT_CHANNEL) as per section 2.1.1 of the
 * specification. This command takes an unsigned int parameter specifying a non-zero channel id. I
 * learned about the _IOW command when I searched for IOCTL commands and found this:
 * https://docs.kernel.org/driver-api/ioctl.html
 * 
 * Note that there are custom error codes added as a comment. They are here as they could be useful for
 * future extentions or debugging.
 */
#ifndef MESSAGE_SLOT_H
#define MESSAGE_SLOT_H

#include <linux/ioctl.h>

/* Component (device and user spaces) names: */
#define KERNEL_MODULE_NAME "message_slot"
#define SENDER_NAME "message_sender"
#define READER_NAME "message_reader"

/* Device configuration: */
#define MAJOR_NUM 235       /* The hard-coded major device number */
#define MAXIMAL_BUFFER 128  /* The maximal message buffer size */
#define SUCCESS 0           /* Standard success return value for kernel operations */
#define CHANNEL_UNSET 0     /* Invalid channel ID */

/* IOCTL command for setting message channel: */
#define MSG_SLOT_CHANNEL _IOW(MAJOR_NUM, 0, unsigned long)

/*
#define DEVICE_OPEN_ERROR -1
#define DEVICE_RELEASE_ERROR -2
#define ALLOCATION_ERROR -3
*/

#endif
