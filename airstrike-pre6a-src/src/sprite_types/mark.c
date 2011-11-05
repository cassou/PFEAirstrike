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
  assert(anim = animation_load(path_to_data("mark.png"),
				  1,16,40));
  animation_last_frame(anim)->trigger = frame_trigger;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof*s);
  s->animation = anim;
  s->state = 0;
  s->anim_p = 0;
  return s;
}

sprite_type_t mark =
  {
    "mark",
    setup,
    create,
    sprite_free,
    0,
    0,
    0,
  };

REGISTER_SPRITE_TYPE(mark)
