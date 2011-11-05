#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"
#include "config.h"
#include "mech.h"

static animation_t *anim;

struct energymeter
{
  sprite_t base;
  sprite_t *tracking;
};

static int setup()
{
  assert(anim = animation_load(path_to_data("gauge.png"),
				  16,1,4000000));
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof(struct energymeter));
  s->animation = anim;
  s->state = 0;
  s->anim_p = 0;
  ((struct energymeter*)s)->tracking = 0;
  return s;
}

static void em_free(sprite_t *s)
{
  sprite_release(&(((struct energymeter*)s)->tracking));
  free(s);
}

static void update(sprite_t *s)
{
  if (sprite_isvalid(&(((struct energymeter*)s)->tracking)))
    {      
      s->anim_p = ((mech_sprite_t *)(((struct energymeter*)s)->tracking))->damage;
      if (s->anim_p < 0)
	s->anim_p = 0;
      if (s->anim_p > anim->nr_pframes - 1)
	s->anim_p = anim->nr_pframes - 1;
    }
  else
    sprite_kill(s);
}

static void sigget(sprite_t *s, int signal, void *data)
{
  switch(signal)
    {
    case SIGNAL_SPRITETARGET:
      sprite_aquire(data,&(((struct energymeter*)s)->tracking));
      break;
    default:
      break;
    }
}

sprite_type_t energymeter =
  {
    "energymeter",
    setup,
    create,
    em_free,
    0,
    0,
    update,
    sigget
  };

REGISTER_SPRITE_TYPE(energymeter)
