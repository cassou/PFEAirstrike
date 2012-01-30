/*
 * network.h
 *
 *  Created on: 14 nov. 2011
 *      Author: cassou
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "players.h"


void network_init();

int network_keymap[MAXPLAYERS][MAX_CONTROLLER_KEYS];

volatile int netStop ;//place to 1 to stop the network
volatile int netStopped ;//place to 1 when network stopped

int networkLoad;
int networkLoadinterval;
int networkport;
int fps;

#endif /* NETWORK_H_ */
