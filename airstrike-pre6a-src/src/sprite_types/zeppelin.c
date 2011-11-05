#include "sprite_types.h"
#include "level.h"
#include "maths.h"
#include "mech.h"

/* how far from the edge the zeppelin will turn around */
#define TURN_DIST 120

/* zeppelin state */
#define GO_LEFT   1
#define	GO_RIGHT  2
#define WRECKED   3	/* or larger value */
#define KILLED    255	/* larger enough */

static animation_t *z_right;
static animation_t *z_turn_lr;
static animation_t *z_left;
static animation_t *z_turn_rl;
static animation_t *z_wreck;


static sprite_t *create(void)
{
	sprite_t *s;
	s = calloc(1,sizeof(mech_sprite_t));

	mech_defaults((mech_sprite_t *)s,0);
	((mech_sprite_t *)s)->rmass = 0.1; /* very heavy */
	((mech_sprite_t *)s)->gravity = 0; /* floats */
	((mech_sprite_t *)s)->air_isotropic = 0.05; /* catches a lot of air */
	s->animation = z_right;
	s->state = GO_RIGHT;
	return s;
}

static void update(sprite_t *s)
{
	mech_sprite_t *ms = (mech_sprite_t *)s;
	
	if (ms->damage > 50) {
		if (s->state < WRECKED) {
			/* explode once & wreck it */
			create_effect(&explosion,s->x - 30, s->y - 15);
			create_effect(&explosion,s->x + 25, s->y - 6);
			create_effect(&explosion,s->x + 10, s->y + 14);
			sprite_set_animation(s, z_wreck);
			s->state = WRECKED;
			ms->gravity = 1; /* drops */
			ms->air_isotropic = 0;
		} else {
			/* explode it second time & remove */
			if (s->state > WRECKED + 8 && s->state != KILLED) {
				create_effect(&explosion,s->x, s->y + 20);
				sprite_alarm(2000,s,SIGNAL_KILL,0);
				s->state = KILLED;
			}
		}
	} else {
		/* goes back and forth */
		if (s->x - TURN_DIST < level.xmin) {
			ms->lin_impulse[0] = 0.1;
			if (s->state == GO_LEFT) {
				sprite_set_animation(s, z_turn_lr);
				s->state = GO_RIGHT;
			}
		}
		if (s->x + TURN_DIST > level.xmax) {
			ms->lin_impulse[0] = -0.1;
			if (s->state == GO_RIGHT) {
				sprite_set_animation(s, z_turn_rl);
				s->state = GO_LEFT;
			}
		}
	}
	mech_update(ms);
}

static void sigget(sprite_t *s, int signal, void *data)
{
	switch(signal) {
	case SIGNAL_DAMAGE:
		((mech_sprite_t *)s)->damage += *(int *)data;
		break;
	case SIGNAL_KILL:
		create_effect(&explosion,s->x,s->y);
		sprite_kill(s);
		break;
	default:
		break;
	}
}

static void wreck_trigger(sprite_t *s)
{
	/* the wreck ticks until it explodes... */
	s->state++;
}

static int setup(void)
{
	struct animation *last;

	z_left = animation_load(path_to_data("zeppelin-left.png"), 1,1,300);
	z_right = animation_load(path_to_data("zeppelin-right.png"), 1,1,300);
	z_turn_lr = animation_load(path_to_data("zeppelin-turn-lr.png"), 1,16,90);
	z_turn_rl = animation_load(path_to_data("zeppelin-turn-rl.png"), 1,16,90);
	z_wreck = animation_load(path_to_data("zeppelin-wreck.png"), 1,6,240);

	assert(z_left);
	assert(z_right);
	assert(z_turn_lr);
	assert(z_turn_rl);
	assert(z_wreck);
	
	animation_last_frame(z_turn_lr)->next_frame = z_right;
	animation_last_frame(z_turn_rl)->next_frame = z_left;

	z_wreck->delay = 1000; /* make first wreck fram quite long */
	
	/* loop the last wreck frame */
	last = animation_last_frame(z_wreck);
	last->trigger = wreck_trigger;
	last->next_frame = last;
	return 0;
}

sprite_type_t zeppelin = {
    "zeppelin",
    setup,
    create,
    sprite_free,
    0,
    0,
    update,
    sigget
};

REGISTER_SPRITE_TYPE(zeppelin)
