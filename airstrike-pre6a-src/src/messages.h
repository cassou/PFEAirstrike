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
	MSG_HELLO_RESPONSE = 1,
	MSG_KEY = 2,
};

typedef struct
{
	int source_id;
	int mess_type;
	int data;
}  __attribute__ ((__packed__)) AS_message_t;

#endif /* MESSAGES_C_ */
