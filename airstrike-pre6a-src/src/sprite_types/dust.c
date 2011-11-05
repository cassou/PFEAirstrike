#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "airstrike.h"
#include "maths.h"

static animation_t *grow;
static animation_t *dissolve;

static void frame_trigger(sprite_t *s)
{
  sprite_kill(s);
}

static int setup()
{
  assert(grow = animation_load(path_to_data("ldust-grow.png"),
				  1,8,50));
  assert(dissolve = animation_load(path_to_data("ldust-dissolve.png"),
				  1,8,100));
  animation_set_alpha(grow,64);
  animation_set_alpha(dissolve,64);
  animation_last_frame(grow)->next_frame = dissolve;
  animation_last_frame(dissolve)->trigger = frame_trigger;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof *s);
  s->animation = grow;
  s->state = 0;
  s->anim_p = 0;
  return s;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  if (this_sprite->state == 0)
    {
      this_sprite->state = 1;  
      sprite_set_animation(this_sprite, dissolve);
    }
}

sprite_type_t ldust =
  {
    "ldust",
    setup,
    create,
    sprite_free,
    collide,
    0,
    0
  };

REGISTER_SPRITE_TYPE(ldust)

static animation_t *sgrow;
static animation_t *sdissolve;

static int ssetup()
{
  assert(sgrow = animation_load(path_to_data("sdust-grow.png"),
				  1,8,50));
  assert(sdissolve = animation_load(path_to_data("sdust-dissolve.png"),
				  1,8,100));
  animation_set_alpha(sgrow,64);
  animation_set_alpha(sdissolve,64);
  animation_last_frame(sgrow)->next_frame = sdissolve;
  animation_last_frame(sdissolve)->trigger = frame_trigger;
  return 0;
}

static sprite_t *screate()
{
  sprite_t *s;
  s = calloc(1,sizeof *s);
  s->animation = sgrow;
  s->state = 0;
  s->anim_p = 0;
  return s;
}

static void scollide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  if (this_sprite->state == 0)
    {
      this_sprite->state = 1;  
      sprite_set_animation(this_sprite, sdissolve);
    }
}

sprite_type_t sdust =
  {
    "sdust",
    ssetup,
    screate,
    sprite_free,
    scollide,
    0,
    0
  };

REGISTER_SPRITE_TYPE(sdust)

static animation_t *dsmokea;

static int dsetup()
{
  assert(dsmokea = animation_load(path_to_data("direxpl.png"),
				  32,8,30));
  animation_set_alpha(dsmokea,200);
  animation_last_frame(dsmokea)->delay = 100;
  animation_last_frame(dsmokea)->trigger = frame_trigger;
  return 0;
}

static sprite_t *dcreate()
{
  sprite_t *s;
  s = calloc(1,sizeof*s);
  s->animation = dsmokea;
  s->state = 0;
  s->anim_p = 0;
  return s;
}

sprite_type_t dsmoke =
  {
    "dsmoke",
    dsetup,
    dcreate,
    sprite_free,
    0,
    0,
    0
  };

REGISTER_SPRITE_TYPE(dsmoke)
