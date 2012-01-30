#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"
#include "config.h"

static animation_t *anim;

static void frame_trigger(sprite_t *s)
{
  sprite_kill(s);
}

static int setup()
{
  CRITICAL(anim = animation_load(path_to_data("firebomb.png"),
				 1,32,30));
  animation_set_alpha(anim,200);
  animation_last_frame(anim)->trigger = frame_trigger;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  float v[2] = {0,-20};
  s = calloc(1,sizeof(sprite_t));
  s->animation = anim;
  sprite_set_vel(s,v);
  return s;
}

sprite_type_t explosion =
  {
    "explosion",
    setup,
    create,
    sprite_free,
    0,
    0,
    0,
  };

REGISTER_SPRITE_TYPE(explosion)
