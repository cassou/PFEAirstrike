#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"
#include "config.h"

static animation_t *grow;
static animation_t *dissolve;

static void frame_trigger(sprite_t *s)
{
  sprite_kill(s);
}

static int setup()
{
  CRITICAL(grow = animation_load(path_to_data("whitesmoke.png"),
				 1,32,60));
  animation_set_alpha(grow,128);
  //animation_set_alpha(dissolve,128);
  // animation_last_frame(grow)->next_frame = dissolve;
  animation_last_frame(grow)->trigger = frame_trigger;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  float v[2] = {0,-10};
  s = calloc(1,sizeof(sprite_t));
  s->animation = grow;
  sprite_set_vel(s,v);
  return s;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  if (this_sprite->state == 0)
    {
      this_sprite->state = 1;  
      sprite_set_animation(this_sprite, dissolve);
    }
}

sprite_type_t puff =
  {
    "puff",
    setup,
    create,
    sprite_free,
    collide,
    0,
    0,
  };

REGISTER_SPRITE_TYPE(puff)
