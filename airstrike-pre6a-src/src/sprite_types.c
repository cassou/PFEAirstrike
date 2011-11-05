#include <stdio.h>
#include "sprite_types.h"
#include "names.h"

int nr_sprite_types = 0;
sprite_type_t *sprite_types_list[MAX_SPRITE_TYPES];
struct names *sprite_types = 0;

#include "sprites_autogen.inc"

int sprite_types_setup()
{
  int i;
  sprite_types_register();
  for (i=0;i<nr_sprite_types;i++)
    {
      ndef(&sprite_types,sprite_types_list[i]->name,
	   sprite_types_list[i]);
    }
  return 0;
}

sprite_t *create_effect(sprite_type_t *type,int x, int y)
{
  sprite_t *s = sprite_create(type);
  sprite_set_pos(s,x,y);
  sprite_group_insert(effects_group,s);
  return s;
}

