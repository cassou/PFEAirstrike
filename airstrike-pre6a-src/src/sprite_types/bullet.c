#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "sprite_types.h"
#include "maths.h"
#include "config.h"
#include "mech.h"
#include "airstrike.h"

static float ttl;
static int damage;

static animation_t *anim;

static void killme(sprite_t *s)
{
  sprite_kill(s);
}

static int setup()
{
  ttl = cfgnum("bullet.ttl",1000);
  damage = cfgnum("bullet.damage",1);
  assert(anim = animation_load(path_to_data("bullet.png"),1,1,ttl));
  animation_last_frame(anim)->trigger = killme;
  return 0;
}

static sprite_t *create()
{
  sprite_t *s;
  s = calloc(1,sizeof*s);
  s->animation = anim;
  s->state = 0;
  s->anim_p = 0;
  return s;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  sprite_t *s;
  float n[2];
  /* Create a smoke effect in the opposite direction*/
  s = sprite_create(&dsmoke);
  sprite_set_pos(s,x,y);
  sprite_get_vel(this_sprite, n);
  s->anim_p = ((vangle(n) + 128) & 255)/8 ;
  sprite_get_vel(other_sprite,n);
  sprite_set_vel(s,n);
  sprite_group_insert(effects_group,s);
  sprite_signal(other_sprite,SIGNAL_DAMAGE,&damage);
  sprite_kill(this_sprite);
  sound_effect(&sound_bullethit,s->x,s->y);
}

static void collide_world(struct sprite *this_sprite, int x, int y)
{
  sprite_t *s;
  float n[2];
  /* Create a smoke effect in the opposite direction*/
  s = sprite_create(&dsmoke);
  sprite_set_pos(s,x,y);
  sprite_get_vel(this_sprite, n);
  s->anim_p = ((vangle(n) + 128) & 255)/8 ;
  sprite_group_insert(effects_group,s);  
  sprite_kill(this_sprite);
}

static void update(sprite_t *s)
{
  float v[2];
  sprite_get_vel(s,v);
  v[1] += 0.8;
  sprite_set_vel(s,v);
  if ((s->x < 0) || (s->y < 0) ||
      (s->x > sprite_global.bg_image->w) ||
      (s->y > sprite_global.bg_image->h))
    {
      sprite_kill(s);
    }
}

sprite_type_t bullet =
  {
    "bullet",
    setup,
    create,
    sprite_free,
    collide,
    collide_world,
    update
  };

REGISTER_SPRITE_TYPE(bullet)
