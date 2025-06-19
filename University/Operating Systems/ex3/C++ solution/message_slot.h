/**
 * @file message_slot.h
 * @brief Header file for the required message_slot.c file, modified to accomodate the message_slot.cpp
 * file insted.
 * 
 * Yes, I know a Linux kernel and C++ don't mix well, but I just wanted to practice some C++, as I
 * already did this assignment in C in the past.
 * Please regard this as a "if you translate this to C, this would be my implementation".
 */

#ifndef MESSAGE_SLOT_H
#define MESSAGE_SLOT_H

#include <linux/ioctl.h>

#define MAJOR_NUM 235
#define MSG_SLOT_CHANNEL _IOW(MAJOR_NUM, 0, unsigned int)

#endif  // MESSAGE_SLOT_H