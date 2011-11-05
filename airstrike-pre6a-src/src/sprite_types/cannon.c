#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"
#include "mech.h"

static animation_t *fire_left;
static animation_t *turn_lr;
static animation_t *fire_right;
static animation_t *turn_rl;

static void shoot_left(sprite_t *s)
{
  float v[2];
  sprite_t *b;
  b = sprite_create(&cannonball);
  sprite_set_pos(b,s->x - 30, s->y - 28);
  v[0] = (frand() + 0.5)*-100;
  v[1] = v[0];
  sprite_set_vel(b,v);
  sprite_group_insert(mech_group,b);
}

static void shoot_right(sprite_t *s)
{
  float v[2];
  sprite_t *b;
  b = sprite_create(&cannonball);
  sprite_set_pos(b,s->x + 30, s->y - 28);
  v[0] = (frand() + 0.5)*100;
  v[1] = -v[0];
  sprite_set_vel(b,v);
  sprite_group_insert(mech_group,b);
}

static void killme(sprite_t *s)
{
  sprite_kill(s);
}

static int setup()
{
  CRITICAL(fire_left = animation_load(path_to_data("cannon-left.png"),
				      1,16,30));
  CRITICAL(fire_right = animation_load(path_to_data("cannon-right.png"),
				      1,16,30));
  CRITICAL(turn_lr = animation_load(path_to_data("cannon-turn-lr.png"),
				      1,16,60));
  CRITICAL(turn_rl = animation_load(path_to_data("cannon-turn-rl.png"),
				      1,16,60));
  animation_last_frame(fire_left)->trigger = shoot_left;
  animation_last_frame(fire_left)->next_frame = turn_lr;
  animation_last_frame(fire_right)->trigger = shoot_right;
  animation_last_frame(fire_right)->next_frame = turn_rl;
  animation_last_frame(turn_lr)->next_frame = fire_right;
  animation_last_frame(turn_rl)->next_frame = fire_left;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof(mech_sprite_t));
  mech_defaults((mech_sprite_t *)s,MECH_IMMOBILE|MECH_INBG);
  ((mech_sprite_t *)s)->rmass = 0.000000001;
  ((mech_sprite_t *)s)->gravity = 0;
  s->animation = fire_left;
  return s;
}

static void update(sprite_t *s)
{

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

sprite_type_t cannon =
  {
    "cannon",
    setup,
    create,
    sprite_free,
    0,
    0,
    update,
    sigget
  };

REGISTER_SPRITE_TYPE(cannon)

static animation_t *ball;

static void ball_frame_trigger(sprite_t *s)
{
  create_effect(&blacksmoke,s->x,s->y);
}


static int ball_setup()
{
  CRITICAL(ball = animation_load(path_to_data("cannonball.png"),
				      1,1,90));
  ball->trigger = ball_frame_trigger;
  return 0;
}

static sprite_t *ball_create()
{
  sprite_t *s;
  s = calloc(1,sizeof(mech_sprite_t));
  mech_defaults((mech_sprite_t *)s,0);
  ((mech_sprite_t *)s)->rmass = 4;
  ((mech_sprite_t *)s)->gravity = 0.25;
  s->animation = ball;
  return s;
}

static void ball_sigget(sprite_t *s, int signal, void *data)
{
  switch(signal)
    {
    case SIGNAL_DAMAGE:
      create_effect(&explosion,s->x,s->y);
      sprite_kill(s);
      break;
    case SIGNAL_LEVELWARP:
      sprite_kill(s);
      break;
    default:
      break;
    }
}

static void ball_update(sprite_t *s)
{
  mech_update((mech_sprite_t *)s);
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  int damage = 5;
  sprite_signal(other_sprite,SIGNAL_DAMAGE,&damage);
  create_effect(&explosion,this_sprite->x,this_sprite->y);
  sprite_kill(this_sprite);
}

static void collide_world(struct sprite *this_sprite, int x, int y)
{
  /* Create a smoke effect in the opposite direction*/
  create_effect(&explosion,this_sprite->x,this_sprite->y);
  sprite_kill(this_sprite);
}

sprite_type_t cannonball =
  {
    "cannonball",
    ball_setup,
    ball_create,
    sprite_free,
    collide,
    collide_world,
    ball_update,
    ball_sigget
  };

REGISTER_SPRITE_TYPE(cannonball)
