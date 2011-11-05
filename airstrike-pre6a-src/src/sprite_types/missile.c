#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "control.h"
#include "maths.h"
#include "mech.h"
#include "airstrike.h"

struct missile
{
  mech_sprite_t base;
  sprite_t *target;
};

static animation_t *anim;

static int setup()
{
  assert(anim = animation_load(path_to_data("missile.png"),
				     64,1,1000000));
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof(struct missile));
  s->animation = anim;
  return s;
}

static void update(sprite_t *s)
{
  float n[2] = {0,1};
  mech_boundary((mech_sprite_t *)s);
  ((mech_sprite_t *)s)->lin_impulse[1] += 2;
  mech_turn_to((mech_sprite_t *)s,n,1);
  mech_update((mech_sprite_t *)s);
}

static void collide(sprite_t *this_sprite, 
		    sprite_t *other_sprite, 
		    int x, int y)
{
  sprite_t *s;
  float n[2];
  
  if (other_sprite->type == &missile) 
    return;
  s = sprite_create(&dsmoke);
  sprite_set_pos(s,x,y);
  sprite_get_vel(this_sprite, n);
  s->anim_p = ((vangle(n) + 128) & 255)/8 ;
  sprite_get_vel(other_sprite,n);
  sprite_set_vel(s,n);
  sprite_group_insert(effects_group,s);
  sprite_kill(this_sprite);
}

static void collide_world(sprite_t *this_sprite, int x, int y)
{
  sprite_t *s;
  
  s = sprite_create(&explosion);
  sprite_set_pos(s,x,y);
  sprite_group_insert(effects_group,s);  
  sprite_kill(this_sprite);
}

static void sigget(sprite_t *s, int signal, void *data)
{
  float v[2] = {0,0};
  sprite_t *p;
  float r[2];
  int sigabs;

  switch(signal)
    {
    case SIGNAL_CANCONTROL:
      *(int *)data = !(s->state & SPRITE_CRASHING);
      break;
    case SIGNAL_DAMAGE:
      ((mech_sprite_t *)s)->damage += *(int *)data;
      break;
    case SIGNAL_UP:
      s->state |= SPRITE_UP;
      s->state &= ~SPRITE_DOWN;
      break;
    case -SIGNAL_UP:
      s->state &= ~SPRITE_UP;
      break;
    case SIGNAL_DOWN:
      s->state |= SPRITE_DOWN;
      s->state &= ~SPRITE_UP;
      break;
    case -SIGNAL_DOWN:
      s->state &= ~SPRITE_DOWN;
      break;
    case SIGNAL_KILL:
      create_effect(&explosion,s->x,s->y);
      sprite_kill(s);
      break;
    default:
      break;
    }
}


sprite_type_t missile =
  {
    "missile",
    setup,
    create,
    sprite_free,
    collide,
    collide_world,
    update,
    sigget
  };

REGISTER_SPRITE_TYPE(missile)
