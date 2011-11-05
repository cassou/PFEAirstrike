#ifndef LEVEL_H
#define LEVEL_H

extern struct level
{
  int xmin,ymin;
  int xmax,ymax;

  /* mech parameters */
  float gravity;
  float soft_impact_level;
  float soft_impact_damage;
  float hard_impact_level;
  float hard_impact_damage;
} level;

int level_setup();

#endif
