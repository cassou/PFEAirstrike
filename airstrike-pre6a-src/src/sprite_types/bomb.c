#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "control.h"
#include "maths.h"
#include "mech.h"
#include "airstrike.h"

static animation_t *anim;

static int setup()
{
  assert(anim = animation_load(path_to_data("bomb.png"),
				     64,1,1000000));
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof(mech_sprite_t));
  s->animation = anim;
  mech_defaults((mech_sprite_t *)s,1);
  ((mech_sprite_t *)s)->angle = 0;
  ((mech_sprite_t *)s)->rmass = 4;
  ((mech_sprite_t *)s)->gravity = 0.25;
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
  int damage = 20;

  if (other_sprite->type == &bomb) 
    return;
  s = sprite_create(&explosion);
  sprite_set_pos(s,x,y);
  sprite_group_insert(effects_group,s);
  sprite_signal(other_sprite,SIGNAL_DAMAGE,&damage);
  sprite_kill(this_sprite);
}

static void collide_world(struct sprite *this_sprite, int x, int y)
{
  sprite_t *s;
  
  s = sprite_create(&explosion);
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
