/*
 * mailbox.h
 *
 *  Created on: Feb 26, 2011
 *      Author: Spenser Gilliland
 */

#ifndef MAILBOX_H_
#define MAILBOX_H_

#include "message.h"

/* MAILBOX_DEFAULT_MESSAGES -
 *
 */
#define MAILBOX_DEFAULT_MESSAGES (10)                       /* Default number of allowed messages in a mailbox. */
#define MAILBOX_DEFAULT_SIZE     (MAILBOX_DEFAULT_SIZE*100) /* Default maximum size of a mailbox */

/* mailbox_ops_t - Operations that can be performed by the admin of a box
 *
 * MASK_ADD - Add a process to the allowed senders.
 * MASK_DEL - Remove a process from the allowed senders.
 * MASK_SET - Set the allowed senders directly.
 * MAX_SIZE - Sets the maximum size of the queue.
 * MAX_MESSAGES - Sets the maximum number of messages in the queue.
 */
enum mailbox_ops_t { MASK_ADD, MASK_DEL, MASK_SET, MAX_SIZE, MAX_MESSAGES };

/* mailbox_t - A description of the mailbox type which acts as a FIFO.
 *
 * head - oldest message received.
 * tail - most recently received message.
 * num_messages - number of messages in the queue.
 * max_messages - maximum number of messages allowed in queue.
 * size - Total used memory by the queue.
 * max_size - Maximum size of the queue.
 * mask - determines if a process is allowed to deposit a message.
 */
typedef struct mailbox {
	message_t* head;
	message_t* tail;
	unsigned int num_messages;
	unsigned int max_messages;
	unsigned long size;
	unsigned long max_size;
	u32_t      mask;
} mailbox_t;

/* mailbox_alloc - creates a new mailbox.
 *
 * Returns:
 *   A pointer to the mailbox or NULL if an error occurred.
 *   Read the global errno to determine the error.
 */
mailbox_t mailbox_alloc();

/* mailbox_enqueue - Adds a message to the queue.
 *
 * mailbox - the mailbox to use.
 * message - a pointer the message you are enqueuing.
 *
 * Returns:
 *   A pointer to the mailbox or NULL if an error occurred.
 *   Read the global errno to determine the error.
 */
mailbox_t mailbox_enqueue(mailbox_t mailbox, message_t* message);

/* mailbox_dequeue - Removes a message from the queue.
 *
 * mailbox - the mailbox to use.
 * message - the message that was dequeued or NULL if there are no more messages.
 *
 * Returns:
 *   A pointer to the mailbox or NULL if an error occurred.
 *   Read the global errno to determine the error.
 */
mailbox_t mailbox_dequeue(mailbox_t mailbox, message_t* message);

/* mailbox_free - Free's all the messages and the mailbox structure.
 *
 * mailbox - the mailbox to use.
 *
 * Returns:
 *   A pointer to the mailbox or NULL if an error occurred.
 *   Read the global errno to determine the error.
 */
mailbox_t mailbox_free(mailbox_t mailbox);

/* mailbox_admin - Performs modifications to a mailbox.
 *
 * mailbox - the mailbox to use.
 * command - refer to the documentation for the mailbox_ops_t.
 * arg - refer to command above.
 *
 * Returns:
 *   A pointer to the mailbox or NULL if an error occurred.
 *   Read the global errno to determine the error.
 */
mailbox_t mailbox_admin(mailbox_t mailbox, enum mailbox_ops_t command, unsigned int arg);

#endif /* MAILBOX_H_ */
