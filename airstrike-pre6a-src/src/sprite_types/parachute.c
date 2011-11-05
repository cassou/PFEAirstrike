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
  assert(anim = animation_load(path_to_data("parachute.png"),
			       1,16,120));
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof(mech_sprite_t));
  s->animation = anim;
  mech_defaults((mech_sprite_t *)s,0);
  ((mech_sprite_t *)s)->gravity = 0.5;
  return s;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{

}

static void collide_world(struct sprite *this_sprite, int x, int y)
{
  sprite_kill(this_sprite);
}

static void update(sprite_t *s)
{
  mech_boundary((mech_sprite_t *)s);
  mech_update((mech_sprite_t *)s);
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

sprite_type_t parachute =
  {
    "parachute",
    setup,
    create,
    sprite_free,
    collide,
    collide_world,
    update,
    sigget
  };

REGISTER_SPRITE_TYPE(parachute)
