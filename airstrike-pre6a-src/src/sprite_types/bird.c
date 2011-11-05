/*
 * (w) 2003 by Eero Tamminen
 * 
 * TODO:
 * - add particle effect for bird kill
 * - set limit for bird speed
 */
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "bonus.h"
#include "sound.h"
#include "maths.h"
#include "level.h"
#include "mech.h"

#define DISTANCE	80	/* distance to keep from level border */


static animation_t *anim;
static sprite_t *follow;


static void limit_max_speed(float *v)
{
	float h2, h, max = 80;

	h2 = v[0]*v[0] + v[1]*v[1];
	if (h2 > max*max) {
		h = sqrt(h2);
		v[0] *= max/h;
		v[1] *= max/h;
	}
}

static int setup(void)
{
	anim = animation_load(path_to_data("bird.png"), 1,8,40);
	assert(anim);
	animation_make_loop(anim);
	return 0;
}

static void init_position(sprite_t *s)
{
	int x, y;
	float v[2];
	x = level.xmax/2 + rand() % (level.xmax/4) - level.xmax/8;
	y = level.ymax/2 + rand() % (level.ymax/4) - level.ymax/8;
	sprite_set_pos(s,x,y);
	v[0] = rand() % 16 - 8;
	v[1] = rand() % 16 - 8;
	sprite_set_vel(s,v);
}

static sprite_t *create(void)
{
	sprite_t *s;
	s = calloc(1,sizeof(mech_sprite_t));
	mech_defaults((mech_sprite_t *)s,0);
	((mech_sprite_t *)s)->rmass = 20;	// very little mass
	((mech_sprite_t *)s)->gravity = 0.0;
	s->animation = anim;
	init_position(s);
	return s;
}

static void bird_free(sprite_t *s)
{
	/* let birds "vote" a new leader */
	if (follow) {
		follow->state = 0;
		sprite_release(&follow);
	}
	sprite_free(s);
}

static void update(sprite_t *s)
{
	float x, y, v[2];
	sprite_t *it;

	if (global_aim.bird) {
		/* all birds follow */
		it = global_aim.bird;
		if (follow) {
			follow->state = 0;
			sprite_release(&follow);
		}
	} else {
		if (follow) {
			it = follow;
		} else {
			/* make me first */
			s->state = 1;
			it = s;
		}
		/* next bird follows me */
		sprite_aquire(s,&follow);
	}

	if (follow && s->state) {
		x = 0;
		y = 0;
		/* keep within screen */
		if (s->x - DISTANCE < level.xmin) {
			x += 0.05;
		}
		if (s->x + DISTANCE > level.xmax) {
			x -= 0.05;
		}
		if (s->y - DISTANCE < level.ymin) {
			y += 0.05;
		}
		if (s->y + 2*DISTANCE > level.ymax) {
			y -= 0.05;
		}
		((mech_sprite_t *)s)->lin_impulse[0] = x;
		((mech_sprite_t *)s)->lin_impulse[1] = y;
	} else {
		/* follow "it" */
		v[0] = (it->x - s->x) * 2;
		v[1] = (it->y - s->y) * 2;
		limit_max_speed(v);
		sprite_set_vel(s, v);
	}
	mech_update((mech_sprite_t *)s);
}

static void sigget(sprite_t *s, int signal, void *data)
{
	mech_sprite_t *ms = (mech_sprite_t *)s;
	int xmax, ymax;
	switch(signal) {
	case SIGNAL_DAMAGE:
		sound_effect(&sound_bird, s->x, s->y);
		ms->damage += *(int *)data;
		if (ms->damage > 3) {
			/* TODO: blood or feather particles */
			
			/* sprite is moved to new position, not removed */
			init_position(s);
			ms->damage = 0;
			
			/* let birds "vote" a new leader */
			if (follow) {
				follow->state = 0;
				sprite_release(&follow);
			}
		}
		break;
	default:
		break;
	}
}

sprite_type_t bird = {
	"bird",
	setup,
	create,
	bird_free,
	0,
	0,
	update,
	sigget
};

REGISTER_SPRITE_TYPE(bird)
