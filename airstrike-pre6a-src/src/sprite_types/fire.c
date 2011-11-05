#include "sprite_types.h"

static animation_t *anim;

static void frame_trigger(sprite_t *s)
{
  sprite_kill(s);
}

static int setup()
{
  CRITICAL(anim = animation_load(path_to_data("fire.png"),
				 1,16,45));
  animation_set_center(anim,8,13);
  animation_set_alpha(anim,200);
  animation_last_frame(anim)->trigger = frame_trigger;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof *s);
  s->animation = anim;
  return s;
}

sprite_type_t fire =
  {
    "fire",
    setup,
    create,
    sprite_free,
    0,
    0,
    0,
  };

REGISTER_SPRITE_TYPE(fire)
