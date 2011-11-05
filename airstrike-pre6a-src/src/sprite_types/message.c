/* 
 * (w) 2003 by Eero Tamminen
 */
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"

enum {
	MSG_BACK,
	MSG_DIE,
	MSG_HURT,
	MSG_KAMIKAZE,
	MSG_SCUM,
	MESSAGES
};
static animation_t *msg[MESSAGES];


static int msg_setup(void)
{
	int i;
	animation_t *anim;
	
	if (!msg[0]) {
		/* object is killed before it moves into another frame */
		anim = animation_load(path_to_data("message.png"),
				      1,MESSAGES,9999);
		assert(anim);
		
		for (i = 0; i < MESSAGES; i++) {
			msg[i] = anim;
			anim = anim->next_frame;
		}
	}
	return 0;
}

static INLINE sprite_t *msg_create(int type)
{
	sprite_t *s = calloc(1,sizeof(sprite_t));
	sprite_alarm(2500,s,SIGNAL_KILL,0);
	s->animation = msg[type];
	return s;
}

static sprite_t *msg_kill_create(void)
{
	return msg_create(MSG_BACK);
}

static sprite_t *msg_damage_create(void)
{
	int type;
	switch(rand() % 2) {
	case 1:
		type = MSG_SCUM;
		break;
	default:
		type = MSG_HURT;
		break;
	}
	return msg_create(type);
}

static sprite_t *msg_fire_create(void)
{
	int type;
	switch(rand() % 2) {
	case 1:
		type = MSG_KAMIKAZE;
		break;
	default:
		type = MSG_DIE;
		break;
	}
	return msg_create(type);
}

static void msg_sigget(sprite_t *s, int signal, void *data)
{
  switch(signal) {
  case SIGNAL_KILL:
    /* message expired */
    sprite_kill(s);
    break;
  default:
    break;
  }
}

sprite_type_t msg_kill = {
  "msg_kill",
  msg_setup,
  msg_kill_create,
  sprite_free,
  0,
  0,
  0,
  msg_sigget
};

sprite_type_t msg_damage = {
  "msg_damage",
  msg_setup,
  msg_damage_create,
  sprite_free,
  0,
  0,
  0,
  msg_sigget
};

sprite_type_t msg_fire = {
  "msg_fire",
  msg_setup,
  msg_fire_create,
  sprite_free,
  0,
  0,
  0,
  msg_sigget
};


REGISTER_SPRITE_TYPE(msg_kill)
REGISTER_SPRITE_TYPE(msg_damage)
REGISTER_SPRITE_TYPE(msg_fire)
