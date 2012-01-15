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

enum log_enum {
	LOG_ERROR = 0,
	LOG_INFO = 1,
	LOG_NETWORK = 2,
	LOG_NETWORK_OUT = 3,
	LOG_NETWORK_IN = 4,
	LOG_MESSAGE = 5,
	LOG_PING_MIN = 6,
	LOG_PING_MAX = 7,
	LOG_PING_AV = 8,
	LOG_CPU = 9,
};



void mylog(int type, char * message, int value);
void logOpen();
void logClose();



#endif /* PROTOTYPE_H_ */
