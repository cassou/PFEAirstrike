#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "control.h"
#include "maths.h"
#include "mech.h"
#include "airstrike.h"
#include "players.h"
#include "plane_flags.h"

static animation_t *anim;
static int bomb_damage;
static int bomb_point;

static int setup()
{

	bomb_point = cfgnum("bomb.point",1);
	bomb_damage = cfgnum("bomb.damage",20);
	assert(anim = animation_load(path_to_data("bomb.png"),
			64,1,1000000));
	return 0;
}

static sprite_t *create(void * owner)
{
	sprite_t *s;
	s = calloc(1,sizeof(mech_sprite_t));
	player_t *p = (player_t *)owner;
	s->owner=owner;
	s->animation = anim;
	mech_defaults((mech_sprite_t *)s,1);
	((mech_sprite_t *)s)->angle = 0;
	((mech_sprite_t *)s)->rmass = 4;
	((mech_sprite_t *)s)->gravity = 2;
	((mech_sprite_t *)s)->air_turnrate = 0.01;
	return s;
}

static void update(sprite_t *s)
{
	mech_update((mech_sprite_t *)s);
}

static void collide(struct sprite *this_sprite,
		struct sprite *other_sprite,
		int x, int y)
{
	sprite_t *s;

	if (other_sprite->type == &bomb)
		return;
	s = sprite_create(&explosion,NULL);
	sprite_set_pos(s,x,y);
	sprite_group_insert(effects_group,s);
	sprite_signal(other_sprite,SIGNAL_DAMAGE,&bomb_damage);
	sprite_signal(other_sprite,SIGNAL_LAST_ENNEMI,this_sprite->owner);
	if (!(other_sprite->state & PLANE_CRASHING)){
		this_sprite->owner->points+=bomb_point;
	}
	sprite_kill(this_sprite);
}

static void collide_world(struct sprite *this_sprite, int x, int y)
{
	sprite_t *s;

	s = sprite_create(&explosion,NULL);
	sprite_set_pos(s,x,y);
	sprite_group_insert(effects_group,s);
	sprite_kill(this_sprite);
}

sprite_type_t bomb =
{
		"bomb",
		setup,
		create,
		sprite_free,
		collide,
		collide_world,
		update
};

REGISTER_SPRITE_TYPE(bomb)
