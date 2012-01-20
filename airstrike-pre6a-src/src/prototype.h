/*
 * prototype.h
 *
 *  Created on: 5 nov. 2011
 *      Author: cassou
 */

#ifndef PROTOTYPE_H_
#define PROTOTYPE_H_


#define DEBUG

#ifdef DEBUG
#define DEBUGPOINT(i)  printf("!!!!!!!!!!!!! Debug point %d\n", i)
#else
#define DEBUGPOINT(i)
#endif


#define ERROR(s)  printf(s)

#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))

extern int playerCount;
extern int teamCount;
extern int inGame;

enum log_enum {
	LOG_ERROR = 0,
	LOG_INFO = 1,
	LOG_NETWORK = 2,
	LOG_NETWORK_OUT = 3,
	LOG_NETWORK_IN = 4,
	LOG_NETWORK_TOTAL_OUT = 5,
	LOG_NETWORK_TOTAL_IN = 6,
	LOG_MESSAGE = 7,
	LOG_PING_MIN = 8,
	LOG_PING_MAX = 9,
	LOG_PING_AV = 10,
	LOG_CPU = 11,
	LOG_FPS = 12,
	LOG_CLIENT_COUNT = 13,
	LOG_NETWORK_DISCONNECT = 14,
	LOG_NETWORK_CONNECT = 15,
};



void mylog(int type, char * message, int value);
void logOpen(char * file);
void logClose();



#endif /* PROTOTYPE_H_ */
