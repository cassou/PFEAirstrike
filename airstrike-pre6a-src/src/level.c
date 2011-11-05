#include "level.h"
#include "config.h"
#include "sprite.h"

struct level level;

int level_setup()
{
  level.xmin = 0;
  level.xmax = sprite_global.bg_image->w;
  level.ymin = 0;
  level.ymax = sprite_global.bg_image->h;
  level.gravity = cfgnum("level.gravity",3);
  level.soft_impact_level = cfgnum("level.soft_impact_level",30);
  level.soft_impact_damage = cfgnum("level.soft_impact_damage",2);
  level.hard_impact_level = cfgnum("level.hard_impact_level",200);
  level.hard_impact_damage = cfgnum("level.hard_impact_damage",10);
  return 0;
}
