/*
 * message_queue.c
 *
 *  Created on: Feb 27, 2011
 *      Author: Spenser Gilliland
 */

message_queue_t* message_queue_alloc() {

}

message_queue_t* message_queue_append(message_t* message, message_queue_t* tail) {
	message_queue_t* new = malloc(sizeof(message_queue_t));

	// Means this is the first one in the queue.
	if(tail == NULL) {


	}

	if(new == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	new->message = message;
	tail->next = new;
	tail = new;

	return tail;
}

message_queue_t* message_queue_remove(message_t* message, message_queue_t* head) {

	message_queue_t* tmp = head->next;

	message = head->message;
	free(head);

	return tmp;
}

message_queue_t* message_queue_free(message_queue_t* queue) {
	message_queue_t* tmp;

	while(queue != NULL) {
		tmp = queue->next;
		free(queue);
		queue = tmp;
	}

	return queue;
}

