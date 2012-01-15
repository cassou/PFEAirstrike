#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"
#include "config.h"
#include "mech.h"
#include "teams.h"

static animation_t *anim[MAXTEAMS];

struct teamicon
{
	sprite_t base;
	sprite_t *tracking;
};

static int setup()
{
	/*assert(anim = animation_load(path_to_data("gauge.png"),
			16,1,4000000));*/
	char cbuf[200];
	int i;
	for (i=0;i<MAXTEAMS;i++){
		sprintf(cbuf,"plane%d.png", i);
		anim[i] = animation_load(path_to_data(cbuf),64,1,100);
	}
	return 0;
}

static sprite_t *create(void * owner)
{
	sprite_t *s;
	int teamid =((team_t *)(owner))->id;
	printf("%d --------------------\n",teamid);
	s = calloc(1,sizeof(struct teamicon));
	s->animation = anim[teamid];
	s->state = 0;
	s->anim_p = 32;
	//	((struct teamicon*)s)->tracking = 0;
	return s;
}

static void em_free(sprite_t *s)
{
	//sprite_release(&(((struct teamicon*)s)->tracking));
	free(s);
}

static void update(sprite_t *s)
{
	/*if (sprite_isvalid(&(((struct teamicon*)s)->tracking)))
	{
		s->anim_p = ((mech_sprite_t *)(((struct teamicon*)s)->tracking))->damage;
		if (s->anim_p < 0)
			s->anim_p = 0;
		if (s->anim_p > anim->nr_pframes - 1)
			s->anim_p = anim->nr_pframes - 1;
	}
	else
		sprite_kill(s);	*/

	//s->anim_p = 32;
}

static void sigget(sprite_t *s, int signal, void *data)
{
	switch(signal)
	{
	/*case SIGNAL_SPRITETARGET:
		sprite_aquire(data,&(((struct teamicon*)s)->tracking));
		break;*/
	default:
		break;
	}
}

sprite_type_t teamicon =
{
		"teamicon",
		setup,
		create,
		em_free,
		0,
		0,
		update,
		sigget
};

REGISTER_SPRITE_TYPE(teamicon)
