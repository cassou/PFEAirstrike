/* 
 * (w) 2003 by Eero Tamminen
 */
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"
#include "mech.h"

static animation_t *anim;
static animation_t *hit_anim;

static int setup(void)
{
  anim = animation_load(path_to_data("airballoon.png"), 1,8,140);
  assert(anim);
  animation_make_loop(anim);
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s;
  s = calloc(1,sizeof(mech_sprite_t));
  mech_defaults((mech_sprite_t *)s,0);
  ((mech_sprite_t *)s)->rmass = 0.4;
  ((mech_sprite_t *)s)->gravity = -0.01;
  s->animation = anim;
  return s;
}

static void update(sprite_t *s)
{
  mech_update((mech_sprite_t *)s);
}

static void sigget(sprite_t *s, int signal, void *data)
{
  float v[2] = {0,0};
  sprite_t *p;
  float r[2];
  int sigabs;

  switch(signal)
    {
    case SIGNAL_DAMAGE:
      ((mech_sprite_t *)s)->damage += *(int *)data;
      ((mech_sprite_t *)s)->gravity *= -1; /*weird but fun */
      break;
    case SIGNAL_KILL:
      create_effect(&explosion,s->x,s->y);
      sprite_kill(s);
      break;
    default:
      break;
    }
}

sprite_type_t airballoon =
  {
    "airballoon",
    setup,
    create,
    sprite_free,
    0,
    0,
    update,
    sigget
  };

REGISTER_SPRITE_TYPE(airballoon)
