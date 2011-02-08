/*
 * cmd.h
 *
 *  Created on: Feb 7, 2011
 *      Author: spenser
 */

#ifndef CMD_H_
#define CMD_H_

typedef struct cmd_list {
   char** argv;
   int    pid;
   int    fd_in;
   int    fd_out;
   struct cmd* next;
} cmd;

cmd* cmd_init(void);

cmd* cmd_add(cmd* head);

void cmd_free(cmd* head);

int cmd_exec(cmd* head);

#endif /* CMD_H_ */
