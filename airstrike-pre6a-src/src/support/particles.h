#ifndef PARTICLES
#define PARTICLES
#include <SDL/SDL.h>

#define MAX_PARTICLES 128

enum particle_type
  {
    PARTICLE_NONE = -1,
    PARTICLE_WHITESMOKE,
  };

void particle_create(enum particle_type type, int x, int y, 
		     float dx, float dy);

#endif
