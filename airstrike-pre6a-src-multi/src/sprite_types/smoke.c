#include "sprite_types.h"
#include "maths.h"

static animation_t *blacksmokea;

static void killme(sprite_t *s)
{
  sprite_kill(s);
}

static int setup()
{
  assert(blacksmokea = animation_load(path_to_data("smoke.png"),
				  1,16,100));
  animation_set_alpha(blacksmokea,128);
  animation_last_frame(blacksmokea)->trigger = killme;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  float v[2] = {0.0,-10.0};
  s = calloc(1,sizeof(sprite_t));
  s->animation = blacksmokea;
  sprite_set_vel(s,v);
  return s;
}

sprite_type_t blacksmoke =
  {
    "blacksmoke",
    setup,
    create,
    sprite_free,
    0,
    0,
    0,
  };

REGISTER_SPRITE_TYPE(blacksmoke)
