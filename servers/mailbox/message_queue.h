/*
 * message_queue.h
 *
 *  Created on: Feb 26, 2011
 *      Author: Spenser Gilliland
 */

#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

typedef struct message_queue {
    message_t* message;
    void** segments;
    int* segment_sizes;
    int num_segments;
	message_queue_t* next;
} message_queue_t;

message_queue_t* message_append(message_t* message, message_queue_t* tail);
message_queue_t* message_remove(message_t* message, message_queue_t* head);

#endif /* MESSAGE_QUEUE_H_ */
