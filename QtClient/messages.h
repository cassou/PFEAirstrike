/*
 * messages.c
 *
 *  Created on: 26 nov. 2011
 *      Author: cassou
 */

#ifndef MESSAGES_C_
#define MESSAGES_C_


enum messages_types {
	MSG_HELLO = 0,
	MSG_PING = 1,
	MSG_KEY = 2,
	MSG_POINTS = 3,
	MSG_NAME = 4,
	MSG_NO_SPACE = 5,
	MSG_TIME2START = 6,
	MSG_DAMAGE = 7,
};

typedef struct
{
	int client_id;
	int mess_type;
	int data;
	char name[32];

}  __attribute__ ((__packed__)) AS_message_t;

#endif /* MESSAGES_C_ */
