#ifndef SOUND_H
#define SOUND_H
#include "config.h"
#ifdef USE_SOUND
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "sprite.h"

typedef struct sound
{
  char *filename;
  int nr_loops;
  Mix_Chunk *wave;
  int init_ok; /* 0: not loaded, 1: loaded ok, -1: failed */
} sound_t;

/* Sound effect names go here. The filenames etc are in sound.c */

extern sound_t sound_enginehumm;
extern sound_t sound_enginebroken;
extern sound_t sound_bombdrop;
extern sound_t sound_gunfire;
extern sound_t sound_mechbump; /* when two sprites collide softly */
extern sound_t sound_mechsmash; /* hard collision */
extern sound_t sound_mechgroundbump; /* soft bg coll */
extern sound_t sound_mechgroundsmash; /* hard collision */
extern sound_t sound_bullethit;
extern sound_t sound_bird;

/* end of sound effect names */

/* Intended for continous sounds, does not report sound ending.  This
   is sort of a convenience device; it can be destroyed without
   needing to 'unregister' it. It should however be stopped (using
   set_sound with sound = 0) before freeing it (It is usually part of
   a sprite) */
typedef struct sound_source
{
  sound_t *sound; /* These are private, don't touch */
  int channel;
} sound_source_t;

int sound_setup();
void sound_update(); /* moves sounds etc */
void sound_set_state(int newstate); /* 0 if off, 1 if on */
void sound_move_ear(int x, int y);
int sound_get_state(); /* 0 if off, 1 if on */

/* One time sound effects. Should be pretty short lived. */
void sound_effect(sound_t *sound, int xpos, int ypos);

/* Set sound = 0 to stop playing. Setting the same sound twice does
   not cause a reset of the sound. */
void sound_source_set_sound(sound_source_t *source, sound_t *sound);
void sound_source_set_pos(sound_source_t *source, int x, int y);

#else /* if not USE_SOUND */

#define sound_setup() 0
#define sound_effect(x,y,z)
#define sound_from_sprite(x,y)
#define sound_get_state() 0
#define sound_set_state(x)
#define sound_update()
#define sound_move_ear(x, y)
#define sound_source_set_sound(x,y)
#define sound_source_set_pos(x,y,z)
#define sound_source_t int

#endif

#endif
