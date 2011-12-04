/*
 * plane_flags.h
 *
 *  Created on: 1 déc. 2011
 *      Author: cassou
 */

#ifndef PLANE_FLAGS_H_
#define PLANE_FLAGS_H_

enum plane_flags /* Different states the biplane can be in */
  {
    PLANE_ACCELERATING = 1,
    PLANE_CRASHING = 2,
    PLANE_UP = 4,
    PLANE_DOWN = 8,
  };

#endif /* PLANE_FLAGS_H_ */
