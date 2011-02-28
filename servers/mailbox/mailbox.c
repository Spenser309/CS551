/*
 * mailbox.c
 *
 *  Created on: Feb 26, 2011
 *      Author: Spenser Gilliland
 */

#include "errno.h"
#include "mailbox.h"

int mailbox_allowed(mailbox_t* mailbox, message_t* message) {
	return !(mailbox->mask & message->source);  // True if allowed, false if not.
}

mailbox_t* mailbox_alloc() {
	mailbox_t* mailbox = malloc(sizeof(mailbox_t));

	mailbox->max_size = MAILBOX_DEFAULT_SIZE;
	mailbox->max_messages = MAILBOX_DEFAULT_MESSAGES;

	mailbox->size = 0;
	mailbox->num_messages = 0;

	if(mailbox == NULL) {
		errno = ENOMEM;
	}

	return mailbox;
}

mailbox_t* mailbox_enqueue(mailbox_t* mailbox, message_t* message) {
	// Test to see if the sender is allowed to send to the mailbox.
	if( mailbox_allowed(mailbox, message)) {
		errno = EPERM;
		return NULL;
	}
    // Make sure this message won't go over the max size of the queue.
	if( mailbox->size + message_size(message) > mailbox->max_size) {
		errno = ESIZE;
		return NULL;
	}
	// Make sure this message won't go over the max number of messages in the queue.
	if(mailbox->num_messages + 1 > mailbox->max_messages) {
		errno = ESIZE;
		return NULL;
	}

	mailbox->tail = message_append(message, mailbox->tail);
	mailbox->size += message_size(message);
	mailbox->num_messages += 1;

	return mailbox;
}

mailbox_t* mailbox_dequeue(mailbox_t* mailbox, message_t* message) {

	if(mailbox->size == 0 || mailbox->head == NULL) {
		errno = ESOMETHING; //TODO: Find a good one for this.
		return NULL;
	}

	mailbox->head = message_remove(message, mailbox->head);
	mailbox->size -= message_size(message);
	mailbox->num_messages -= 1;

	return mailbox;
}

mailbox_t* mailbox_free(mailbox_t* mailbox) {

	message_t* tmp;

	while(mailbox->head != NULL) {
		tmp = (mailbox->head)->next;
		free(mailbox->head);
		mailbox->head = tmp;
	}

	return free(mailbox);
}

mailbox_t* mailbox_admin(mailbox_t* mailbox, enum mailbox_ops_t command, unsigned int arg) {
	switch(command) {
	case MASK_ADD:
		mailbox->mask =  (1 << arg) | mailbox->mask;
		break;
	case MASK_DEL:
		mailbox->mask = ~(1 << arg) & mailbox->mask;
		break;
	case MASK_SET:
		mailbox->mask = arg;
		break;
	case MAX_SIZE:
		mailbox->max_size = arg;
		break;
	case MAX_MESSAGES:
		mailbox->max_messages = arg;
		break;
	default:
		mailbox = NULL;
	    errno = ENOTSUP;
		break;
	}

	return mailbox;
}
