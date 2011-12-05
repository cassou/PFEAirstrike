#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "control.h"
#include "maths.h"
#include "mech.h"
#include "airstrike.h"
#include "plane_flags.h"
#include "teams.h"
#include "players.h"


struct biplane
{
	mech_sprite_t sprite; /* the biplane is also a mech sprite */
	sprite_type_t *bullet_type;
	sprite_timer_t gun_timer; /* keeps track of when it last fired */
	sprite_timer_t bomb_timer;
	int nr_bombs;
};

/* some parameters we get from config files */
static float engine_strength;
static float turn_amount;
static float bomb_delay;
static float bullet_delay;
static float hitpoints;
static float mass;
static float air_isotropic;
static float air_turnrate;
static float air_normal;
static int nr_bombs;

static animation_t *right_anim[MAXTEAMS];
static animation_t *crashing[MAXTEAMS];

static void frame_trigger(sprite_t *s)
{
	if (s->state & PLANE_ACCELERATING)
	{
		if (((mech_sprite_t *)s)->damage < 10){}
		/*FUME*/
		//create_effect(&puff,s->x,s->y);
		else
			create_effect(&blacksmoke,s->x,s->y);
	}
}

static void crashing_trigger(sprite_t *s)
{
	sprite_t *p;
	float v[2] = {0,-20};
	p = sprite_create(&blacksmoke,NULL);
	sprite_set_pos(p,s->x,s->y);
	sprite_set_vel(p,v);
	sprite_group_insert(effects_group,p);
	create_effect(&fire,s->x,s->y);
}





static int setup(void * owner)
{
	int i;
	char cbuf[200];
	for (i=0;i<MAXTEAMS;i++){


		sprintf(cbuf,"plane%d.png", i);
		CRITICAL(right_anim[i] = animation_load(path_to_data(cbuf),
				64,1,100));
		sprintf(cbuf,"plane%dwreck.png", i);
		CRITICAL(crashing[i] = animation_load(path_to_data(cbuf),
				64,1,180));

		animation_make_loop(right_anim[i]);
		animation_make_loop(crashing[i]);
		right_anim[i]->trigger = frame_trigger;
		crashing[i]->trigger = crashing_trigger;
	}

	engine_strength = cfgnum("blueplane.engine_strength",5);
	turn_amount = cfgnum("blueplane.turn_amount",0.02);
	bomb_delay = cfgnum("blueplane.bomb_delay",1000);
	bullet_delay = cfgnum("blueplane.bullet_delay",140);
	hitpoints = cfgnum("blueplane.hitpoints",15);
	mass = cfgnum("blueplane.mass",1);
	air_isotropic = cfgnum("blueplane.air_isotropic",0.00005);
	air_turnrate = cfgnum("blueplane.air_turnrate",0.00005);
	air_normal = cfgnum("blueplane.air_normal",0.001);
	nr_bombs = cfgnum("blueplane.nr_bombs",5);
	return 0;
}

static sprite_t *create(void * owner)
{
	sprite_t *s;
	player_t *p = (player_t *)owner;
	CRITICAL(s = calloc(1,sizeof(struct biplane)));
	s->animation = right_anim[p->team->id];
	s->anim_p = 32;
	s->owner=p;
	mech_defaults((mech_sprite_t *)s,MECH_CANROTATE);
	((struct biplane *)s)->bullet_type = &bullet;
	((struct biplane *)s)->nr_bombs = nr_bombs;
	((mech_sprite_t *)s)->rmass = 1/mass;
	((mech_sprite_t *)s)->gravity = mass;
	((mech_sprite_t *)s)->angle = 128;

	((mech_sprite_t *)s)->air_isotropic = air_isotropic;
	((mech_sprite_t *)s)->air_turnrate = air_turnrate;
	((mech_sprite_t *)s)->air_normal = air_normal;
	return s;
}

static void update(sprite_t *s)
{
	float v[2],x,pull[2];
	mech_sprite_t *ms = (mech_sprite_t *)s;

	sprite_get_vel(s,v);
	if (!(s->state & PLANE_CRASHING))
	{
		if (s->state & PLANE_ACCELERATING)
		{
			vset(pull,mech_heading(ms));
			vrot(pull,-16); /* maybe the angle should depend on up/down */
			vmadd(ms->lin_impulse,engine_strength,pull);
		}
		if (s->state & PLANE_UP)
			ms->air_rotate = -turn_amount;
		else if (s->state & PLANE_DOWN)
			ms->air_rotate = turn_amount;
		else
			ms->air_rotate = 0;

		if (ms->damage >= hitpoints)
		{
			s->state |= PLANE_CRASHING;
			sprite_set_animation(s,crashing[0]);//TODO replace the zero
			create_effect(&fire,s->x,s->y);
			sprite_alarm(7000,s,SIGNAL_KILL,0);
		}
	}
	mech_update(ms);
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
		*(int *)data = !(s->state & PLANE_CRASHING);
		break;
	case SIGNAL_DAMAGE:
		((mech_sprite_t *)s)->damage += *(int *)data;
		break;
	case SIGNAL_ACCELERATE:
		s->state |= PLANE_ACCELERATING;
		break;
	case -SIGNAL_ACCELERATE:
		s->state &= ~PLANE_ACCELERATING;
		break;
	case SIGNAL_UP:
		s->state |= PLANE_UP;
		s->state &= ~PLANE_DOWN;
		break;
	case -SIGNAL_UP:
		s->state &= ~PLANE_UP;
		break;
	case SIGNAL_DOWN:
		s->state |= PLANE_DOWN;
		s->state &= ~PLANE_UP;
		break;
	case -SIGNAL_DOWN:
		s->state &= ~PLANE_DOWN;
		break;
	case SIGNAL_FIRE: /* create bullet */
		if (sprite_timer_finished(((struct biplane*)s)->gun_timer) &&
				!(s->state & PLANE_CRASHING))
		{
			sound_effect(&sound_gunfire,s->x,s->y);
			p = sprite_create(((struct biplane*)s)->bullet_type,s->owner);
			sprite_group_insert(bullet_group,p);
			r[0] = mech_heading((mech_sprite_t *)s)[0];
			r[1] = mech_heading((mech_sprite_t *)s)[1];
			vmul(r,21); /* Find start of bullet in clumsy way */
			vrot(r,-9);
			sprite_set_pos(p,s->x + r[0],s->y + r[1]);
			sprite_get_vel(s,v);
			vmadd(v,100,mech_heading((mech_sprite_t *)s));
			sprite_set_vel(p,v);
			/* cannot fire again in some time */
			sprite_timer_set(&(((struct biplane*)s)->gun_timer),bullet_delay);
		}
		break;
	case SIGNAL_NUM0: /* create bomb */
		if (sprite_timer_finished(((struct biplane*)s)->bomb_timer) &&
				(!(s->state & PLANE_CRASHING)) &&
				(((struct biplane*)s)->nr_bombs > 0))
		{
			((struct biplane*)s)->nr_bombs--;
			p = sprite_create(&bomb,s->owner);
			p->anim_p = s->anim_p;
			((mech_sprite_t *)p)->angle = ((mech_sprite_t *)s)->angle;
			r[0] = mech_heading((mech_sprite_t *)s)[0];
			r[1] = mech_heading((mech_sprite_t *)s)[1];
			vmul(r,14);
			vrot(r,-80);
			sprite_set_pos(p,s->x + r[0],s->y + r[1]);
			sprite_get_vel(s,v);
			vmadd(v,5,r);
			sprite_set_vel(p,v);
			sprite_group_insert(mech_group,p);
			sprite_timer_set(&(((struct biplane*)s)->bomb_timer),
					bomb_delay);
		}
		break;
	case SIGNAL_NUM1: /* jump ship */
		if (sprite_timer_finished(((struct biplane*)s)->bomb_timer) &&
				(!(s->state & PLANE_CRASHING)))
		{
			p = sprite_create(&parachute,NULL);
			r[0] = mech_heading((mech_sprite_t *)s)[0];
			r[1] = mech_heading((mech_sprite_t *)s)[1];
			vmul(r,14);
			vrot(r,80);
			sprite_set_pos(p,s->x + r[0],s->y + r[1]);
			sprite_get_vel(s,v);
			vmadd(v,5,r);
			sprite_set_vel(p,v);
			sprite_group_insert(mech_group,p);
			sprite_timer_set(&(((struct biplane*)s)->bomb_timer),bomb_delay);
		}
		break;
	case SIGNAL_KILL:
		create_effect(&explosion,s->x,s->y);
		sprite_kill(s);
		break;
	case SIGNAL_ISHARMLESS:
		if (s->state & PLANE_CRASHING)
			((struct signal_reply *)data)->reply = 1;
		break;
	case SIGNAL_STATSTRING://TTODO : useless ?
		sprintf(data,"%i bombs",((struct biplane*)s)->nr_bombs);
		break;
	default:
		break;
	}
}

static void freebip(sprite_t *s)
{
	free(s);
}

static void collide(struct sprite *this_sprite,
		struct sprite *other_sprite,
		int x, int y)
{
	printf("Colliding with a %s\n",other_sprite->type->name);
}

static void collide_world(struct sprite *this_sprite, int x, int y)
{
	printf("Colliding with background\n");
}

sprite_type_t blueplane =
{
		/*.name = */ "blueplane",
		/*.setup = */ setup,
		/*.create_sprite =*/ create,
		/*.free_sprite =*/ freebip,
		/*.collide =*/ 0,
		/*.bg_collide =*/ 0,
		/*.update =*/ update,
		/*.signal = */sigget
};

REGISTER_SPRITE_TYPE(blueplane)
