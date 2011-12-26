/*
 * keys.h
 *
 *  Created on: 26 nov. 2011
 *      Author: cassou
 */

#ifndef KEYS_H_
#define KEYS_H_

#define MAX_CONTROLLER_KEYS 6
enum keys {
	KEY_NONE = 0,/**do nothing but avoid a bug because of -0==0 and the way keys are send by clients**/
	KEY__UP = 1,
	KEY__DOWN = 2,
	KEY__ACCELERATE = 3,
	KEY_BOMB = 4,
	KEY_FIRE = 5
};

#endif /* KEYS_H_ */
