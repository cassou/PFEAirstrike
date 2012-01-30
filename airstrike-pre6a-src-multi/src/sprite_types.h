#ifndef SPRITE_TYPES
#define SPRITE_TYPES

#include <malloc.h>
#include <assert.h>
#include "sprite.h"
#include "sound.h"
#include "config.h"
#include "names.h"
#include "signals.h"
#include "airstrike.h"
#include "sprites_autogen.h"
#include "console.h"
#include "utils.h"

/* 
 * This file defines sort of a sprite type registry, where the
 * different types can be dynamically accessed. The idea is that
 * sometimes it might be nice to parse levels without compiling, for
 * example.  Use the sprite_types namespace to access the types. (see
 * names.h for more info) sprite_types_list is used by the sprite
 * types to register themself.
 */

#define MAX_SPRITE_TYPES 64

extern int nr_sprite_types;
extern sprite_type_t *sprite_types_list[MAX_SPRITE_TYPES];
extern struct names *sprite_types;

/* 
 * This macro should be used at file scope to register each sprite type
 * The argument is the name of the struct itself, not a pointer. No 
 * semicolon required
 */
#define REGISTER_SPRITE_TYPE(t)

/* Call the setup function of each registered type */
int sprite_types_setup();

/* Utility functions used by the sprite types: */

sprite_t *create_effect(sprite_type_t *type,int x, int y);

static void sprite_free(sprite_t *s)
{
  free(s);
}


enum sprite_flags /* Different states the sprite might be in */
  {
    SPRITE_ACCELERATING = 1,
    SPRITE_CRASHING = 2,
    SPRITE_UP = 4,
    SPRITE_DOWN = 8,
  };


#endif
