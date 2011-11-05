#ifndef MECHANICS_H
#define MECHANICS_H
#include "sprite.h"
#include "maths.h"

/* The angular resolution of the sprites,
 * in parts of 256 of a whole circle 
 */
#define MECH_ANG_DIV 4

/* sprite flags: Defaults should be flag not set */
#define MECH_CANROTATE 1 /* If the sprite can rotate */
#define MECH_INBG 2 /* If the sprite can be inside the background */
#define MECH_IMMOBILE 4

typedef struct mech_sprite
{
  sprite_t sprite;
  unsigned int mech_flags;
  float rmass; /* 1/mass. Used in collisions*/
  float gravity; /*How much it is affected by gravity(for balloons etc)*/
  float rinmom; /* 1/I, rotational inertial moment */

  float air_turnrate;
  float air_isotropic;
  float air_normal;
  float air_rotate;

  float angle;
  float ang_vel;
  float ang_impulse;
  float lin_impulse[2]; /* dv/timestep */
  int damage;
} mech_sprite_t;

/* Returns a vector pointing in the heading of sprite s */
static INLINE float *mech_heading(mech_sprite_t *s)
{
  return trig((int)s->angle);
}

static INLINE float *mech_right(mech_sprite_t *s)
{
  return trig((int)s->angle + 64);
}

/* Set the physical constants to default sane values */
void mech_defaults(mech_sprite_t *s,unsigned int flags);

void mech_turn_to(mech_sprite_t *s, const float *n, float strength);
/* Returns the velocity of point r on sprite s */
void mech_point_vel(mech_sprite_t *s, const float *p, float *v);
void mech_apply_impulse(mech_sprite_t *s, const float *p, const float *i);
/* The 'effective' mass of sprite when pushed at p in dir n (|n| = 1)! */
float mech_eff_rmass(mech_sprite_t *s, const float *p, const float *n);
void mech_sprite_collide(sprite_t *this_sprite,
			 sprite_t *other_sprite,
			 int x, int y);
void mech_sprite_bg_collide(sprite_t *s, int x, int y);
void mech_wind(float *v);
void mech_aerodynamics(sprite_t *s);
void mech_update(mech_sprite_t *s);
void mech_boundary(mech_sprite_t *ms);
void mech_gravity(sprite_group_t *group);

#endif
