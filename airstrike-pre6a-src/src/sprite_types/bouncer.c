#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"
#include "mech.h"

static animation_t *anim;
static animation_t *hit_anim;
static animation_t *deflate_anim;

static void frame_trigger(sprite_t *s)
{
  sprite_set_animation(s,anim);
  s->state = 0;
}

static void killme(sprite_t *s)
{
  sprite_t *b;
  b = sprite_create(&bonus);
  sprite_set_pos(b,s->x, s->y);
  /* The bonus is not really a bullet, but has the same
     properties collision-wise */
  sprite_group_insert(bullet_group,b);
  sprite_kill(s);
}

static int setup()
{
  CRITICAL(anim = animation_load(path_to_data("balloon-float.png"),
			       1,1,33));
  CRITICAL(hit_anim = animation_load(path_to_data("balloon-bounce.png"),
			       1,32,33));
  CRITICAL(deflate_anim = 
	   animation_load(path_to_data("balloon-deflate.png"),1,32,30));
  animation_last_frame(hit_anim)->trigger = frame_trigger;
  animation_last_frame(deflate_anim)->trigger = killme;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof(mech_sprite_t));
  mech_defaults((mech_sprite_t *)s,0);
  ((mech_sprite_t *)s)->rmass = 3;
  ((mech_sprite_t *)s)->gravity = 0.0;
  s->animation = anim;
  return s;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  if (this_sprite->state == 0)
    {
      sprite_set_animation(this_sprite, hit_anim);
      this_sprite->state = 1;
    }
}

static void update(sprite_t *s)
{
  mech_boundary((mech_sprite_t *)s);
  mech_update((mech_sprite_t *)s);
  if ((((mech_sprite_t *)s)->damage > 5) && 
      (s->state != 3))
    {
      s->state = 3;
      sprite_set_animation(s,deflate_anim);
    }
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

sprite_type_t bouncer =
  {
    "bouncer",
    setup,
    create,
    sprite_free,
    collide,
    0,
    update,
    sigget
  };

REGISTER_SPRITE_TYPE(bouncer)
