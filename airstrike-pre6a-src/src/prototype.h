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

extern int playerCount;

#endif /* PROTOTYPE_H_ */
