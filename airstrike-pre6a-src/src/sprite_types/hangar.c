#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"
#include "mech.h"

static animation_t *anim;

static void frame_trigger(sprite_t *s)
{
  sprite_set_animation(s,anim);
  s->state = 0;
}

static void killme(sprite_t *s)
{
  sprite_kill(s);
}

static int setup()
{
  assert(anim = animation_load(path_to_data("hangar.png"),
			       8,1,330000));
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof(mech_sprite_t));
  mech_defaults((mech_sprite_t *)s,MECH_IMMOBILE|MECH_INBG);
  ((mech_sprite_t *)s)->rmass = 0.000000001;
  ((mech_sprite_t *)s)->gravity = 0;
  s->animation = anim;
  return s;
}

static void update(sprite_t *s)
{
  s->anim_p = ((mech_sprite_t *)s)->damage/10;
  if (s->anim_p > 7)
    s->anim_p = 7;
}

static void sigget(sprite_t *s, int signal, void *data)
{
  switch(signal)
    {
    case SIGNAL_DAMAGE:
      ((mech_sprite_t *)s)->damage += *(int *)data;
      break;
    default:
      break;
    }
}

sprite_type_t hangar =
  {
    "hangar",
    setup,
    create,
    sprite_free,
    0,
    0,
    update,
    sigget
  };

REGISTER_SPRITE_TYPE(hangar)
