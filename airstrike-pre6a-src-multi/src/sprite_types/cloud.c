#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"

static animation_t *drift;

static int setup()
{
  assert(drift = animation_load(path_to_data("cloud.png"),1,1,1000000));
  animation_last_frame(drift)->next_frame = drift;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof(sprite_t));
  s->animation = drift;
  return s;
}

static void update(sprite_t *s)
{
  if (s->x > sprite_global.bg_image->w + 200)
    {
      s->x = - 200;
    }
}

sprite_type_t cloud =
  {
    "cloud",
    setup,
    create,
    sprite_free,
    0,
    0,
    update
  };

REGISTER_SPRITE_TYPE(cloud)
