/*
 * network.h
 *
 *  Created on: 14 nov. 2011
 *      Author: cassou
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "players.h"

typedef struct
{
	int source_id;
	int mess_type;
	int data;
}message_t;

void network_init();

int network_keymap[MAXPLAYERS][MAX_CONTROLLER_KEYS];

#endif /* NETWORK_H_ */
