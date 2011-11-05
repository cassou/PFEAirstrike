/* 
 * Bonus machine and bonus objects
 * 
 * (w) 2003 by Eero Tamminen
 * 
 * TODO:
 * - Implement aiming according to 'bird' example to
 *   cannon, cloud & ufo objects
 * - Check that other_sprite can be only player sprite +
 *   make it call global_aim_clear(sprite_t*) when killed
 * - Implement different bonus effects
 */
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "bonus.h"
#include "maths.h"
#include "mech.h"

/* ------------------------------------------------------------ */

static animation_t *machine;
static animation_t *machine_open;


static void shoot_bonus(sprite_t *s)
{
	float v[2] = {0,20};
	sprite_t *b;

	b = sprite_create(&bouncer);
	sprite_set_pos(b, s->x, s->y + 32);
	sprite_set_vel(b,v);
	sprite_group_insert(mech_group,b);
}

static int machine_setup(void)
{
	animation_t *last;

	machine = animation_load(path_to_data("bonus-machine.png"), 1,16,60);
	assert(machine);

	/* last frame shoots a bonus */
	animation_last_frame(machine)->trigger = shoot_bonus;
	animation_make_loop(machine);

	/* show first frame until we get a signal */
	machine_open = machine->next_frame;
	machine->next_frame = machine;
	return 0;
}

static sprite_t *machine_create(void)
{
	sprite_t *s;
	s = calloc(1,sizeof(mech_sprite_t));
	mech_defaults((mech_sprite_t *)s,MECH_IMMOBILE|MECH_INBG);
	((mech_sprite_t *)s)->rmass = 0.000000001;
	((mech_sprite_t *)s)->gravity = 0;
	sprite_alarm(10000,s,SIGNAL_FIRE,0);
	s->animation = machine;
	return s;
}

static void machine_sigget(sprite_t *s, int signal, void *data)
{
	switch(signal) {
	case SIGNAL_FIRE:
		/* proceed with machine animation */
		sprite_set_animation(s, machine_open);
		sprite_alarm(30000,s,SIGNAL_FIRE,0);
		break;
	default:
		break;
	}
}

sprite_type_t bonusmachine = {
	"bonusmachine",
	machine_setup,
	machine_create,
	sprite_free,
	0,
	0,
	0,
	machine_sigget
};


/* ------------------------------------------------------------ */

static animation_t *bonuses[BONUSES];

struct global_aim_t global_aim  = {
	/* .airballoon =*/ NULL,
	/* .bird =*/ NULL,
	/* .cannon =*/ NULL,
	/* .cloud =*/ NULL,
	/* .ufo =*/ NULL,
	/* .zeppelin =*/ NULL
};

static INLINE void clear_it(sprite_t **pos, sprite_t *it)
{
  if (*pos == it) 
    {
      sprite_release(pos);
    }
}

void global_aim_clear(sprite_t *it)
{
	clear_it(&(global_aim.airballoon), it);
	clear_it(&(global_aim.bird), it);
	clear_it(&(global_aim.cannon), it);
	clear_it(&(global_aim.cloud), it);
	clear_it(&(global_aim.ufo), it);
	clear_it(&(global_aim.zeppelin), it);
}

static INLINE void toggle_it(sprite_t **pos, sprite_t *it)
{
  if (*pos == it) {
    sprite_release(pos);
  } else {
    if (*pos) {
      sprite_release(pos);
    }
    sprite_aquire(it,pos);
  }
}

static void bonus_collide(struct sprite *this_sprite, 
			  struct sprite *other_sprite, 
			  int x, int y)
{
	/* TODO:
	 * - if other_sprite is not player's sprite, return
	 * - make 'it' be the player's opponent
	 */
	sprite_t *it = other_sprite;
	
	/* TODO: implement bonus effects */
	switch (this_sprite->state) {
	case BONUS_AIRBALLOON:
		toggle_it(&(global_aim.airballoon), it);
		break;
	case BONUS_BIRD:
		toggle_it(&(global_aim.bird), it);
		break;
	case BONUS_CANNON:
		toggle_it(&(global_aim.cannon), it);
		break;
	case BONUS_CLOUD:
		toggle_it(&(global_aim.cloud), it);
		break;
	case BONUS_UFO:
		toggle_it(&(global_aim.ufo), it);
		break;
	case BONUS_ZEPPELIN:
		toggle_it(&(global_aim.zeppelin), it);
		break;
		
	case BONUS_SCORE:
		/* TODO: increase players score */
		break;
	case BONUS_LIFE:
		/* TODO: add a life for the player */
		break;
	default:
		break;
	}
	sprite_kill(this_sprite);
}

static int bonus_setup(void)
{
	int i;
	animation_t *anim;

	/* object is killed before it moves into another frame */
	anim = animation_load(path_to_data("bonus.png"), 1,BONUSES,9999);
	assert(anim);

	for (i = 0; i < BONUSES; i++) {
		bonuses[i] = anim;
		anim = anim->next_frame;
	}
	return 0;
}

static sprite_t *bonus_create(void)
{
  sprite_t *s;
  s = calloc(1,sizeof(*s));
  sprite_alarm(5000,s,SIGNAL_KILL,0);
  s->state = rand() % BONUSES;
  s->animation = bonuses[s->state];
  return s;
}

static void bonus_sigget(sprite_t *s, int signal, void *data)
{
  switch(signal) {
  case SIGNAL_KILL:
    /* bonus expired */
    sprite_kill(s);
    break;
  default:
    break;
  }
}

sprite_type_t bonus = {
	"bonus",
	bonus_setup,
	bonus_create,
	sprite_free,
	bonus_collide,
	0,
	0,
	bonus_sigget
};


REGISTER_SPRITE_TYPE(bonusmachine)
REGISTER_SPRITE_TYPE(bonus)
