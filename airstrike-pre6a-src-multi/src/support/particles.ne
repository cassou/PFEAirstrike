#include "particles.h"
#include "sprite.h"

static struct particle_spec
{
  char *animation_file;
  int nr_frames;
  int frame_time;
  SDL_Surface **frames;
} particle_spec[] =
  {
    {"whitesmoke.png",32},
  };

static struct particle
{
  enum particle_type type;
  Uint32 timestamp;
  int x,y;
  int dx,dy;
} particles[MAX_PARTICLES];

static int ipar = 0;

void particle_create(enum particle_type type, int x, int y, 
		     float dx, float dy)
{
  particles[ipar].type = type;
  particles[ipar].timestamp = sprite_global.game_clock;
  particles[ipar].x = NUM2FIX(x);
  particles[ipar].y = NUM2FIX(y);
  particles[ipar].dx = NUM2FIX(dx);
  particles[ipar].dy = NUM2FIX(dy);
}

void particles_draw()
{
  int p;
  for (p=0;p<MAX_PARTICLES;p++)
    if (particles[p].type > PARTICLE_NONE)
      {
	particles[ipar].x += particles[ipar].dx;
	particles[ipar].y += particles[ipar].dy;
	switch (particles[p].type)
	  {
	    
	  default:
	    fprintf(stderr,"particles_draw() Unknown type %i.\n",
		    particle[p].type);
	    break;
	  }
      }
}
