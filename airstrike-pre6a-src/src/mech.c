#include "mech.h"
#include "sprite_types.h"
#include "level.h"
#include "winds.h"
#include <math.h>

void mech_defaults(mech_sprite_t *s, unsigned int flags)
{
  s->rmass = 1; 
  if (flags & MECH_CANROTATE)
    s->rinmom = 2.0;
  else
    s->rinmom = 0;
  s->gravity = 1;
  s->ang_vel = 0;
  s->ang_impulse = 0;

  s->air_turnrate = 0.0001;
  s->air_isotropic = 0.00001;
  s->air_normal = 0.0001;
  s->air_rotate = 0;

  s->mech_flags = flags;
}

void mech_air(mech_sprite_t *s)
{
  float iso,vnorm,rot,v[2];

  /* TODO: fix this to work properly and take into
   * account object size & mass.
   */
  float wind[2];
  sprite_t *ss = (sprite_t *)s;

  get_object_wind(ss->x, ss->y, wind);
  sprite_get_vel(ss, v);
  v[0] += wind[0];
  v[1] += wind[1];
  vset(v,ss->vel);

  vnorm = vdot(mech_right(s),v);
  vnorm = -sgn(vnorm)*square(vnorm)*s->air_normal;

  iso = -sqrt(vdot(v,v))*(s->air_isotropic);

  vmadd(s->lin_impulse,iso,v);
  vmadd(s->lin_impulse,vnorm,mech_right(s));

  rot = vdot(mech_heading(s),v);
  if (rot > 0)
    {
      s->ang_impulse += rot*s->air_rotate;
    }
  rot = vcross(mech_heading(s),v);
  s->ang_impulse += sgn(rot)*square(rot)*s->air_turnrate;

}

void mech_gravity(sprite_group_t *group)
{
  sprite_t *s;
  int i;
  for (i=0;i<group->nr_sprites;i++)
    group->sprites[i]->vel[1] += 
      level.gravity*(((mech_sprite_t *)(group->sprites[i]))->gravity);
}

void mech_update(mech_sprite_t *s)
{
  float v[2];
  if (!(s->mech_flags & MECH_IMMOBILE))
    {
      mech_boundary(s);
      mech_air(s);
      vmadd(((sprite_t *)s)->vel,s->rmass,s->lin_impulse);
      s->lin_impulse[0] = 0;
      s->lin_impulse[1] = 0;
      //s->lin_impulse[1] = level.gravity*s->gravity; /* gravity */
    }
  if (s->mech_flags & MECH_CANROTATE)
    {
      s->ang_vel += s->ang_impulse*s->rinmom;
      s->ang_vel *= 0.95;
      s->angle += s->ang_vel;
      s->sprite.anim_p = (((int)(s->angle + 0.5)) & 255) / MECH_ANG_DIV;
      s->ang_impulse = 0;
    }
}

/* this one is obsolete */
void mech_turn_to(mech_sprite_t *s, const float *n, float strength)
{
  return;
  if (s->mech_flags & MECH_CANROTATE)
    {
      /* replace by actual angular impulse instead */
      if (vdot(n,mech_heading(s)) > 0)
	s->angle -= strength*vcross(n,mech_heading(s));
      else
	s->angle += strength*vcross(n,mech_heading(s));
    }
}

/* nonrotating bodies only for now */
void mech_point_vel(mech_sprite_t *s, const float *p, float *v)
{
  vset(v,((sprite_t *)s)->vel);
  return;
  if (s->mech_flags & MECH_CANROTATE)
    {
      v[1] += 0.024543692606170259*s->ang_vel*(p[0] - s->sprite.x);
      v[0] += 0.024543692606170259*s->ang_vel*(s->sprite.y - p[1]);
    }
}

void mech_apply_impulse(mech_sprite_t *s, const float *p, const float *i)
{
  vadd(s->lin_impulse,i);
  return;
  if (s->mech_flags & MECH_CANROTATE)
    {
      float d[2];
      d[0] = p[0] - s->sprite.x;
      d[1] = p[1] - s->sprite.y;
      /* hmm */
      s->ang_impulse += vcross(d,i);
    }
}

/* Return 1/effective mass of point p on s when pushed in dir n*/
float mech_eff_rmass(mech_sprite_t *s, const float *p, const float *n)
{
  return s->rmass;
  if (!(s->mech_flags & MECH_CANROTATE))
    {
      return s->rmass;
    }
  else
    { 
      /* if the object can rotate then the force will
	 cause both rotation and translation, and we need to
	 distribute the "push" */
      float d[2];
      d[0] = p[0] - s->sprite.x;
      d[1] = p[1] - s->sprite.y;
      /* correct ? */
      return fabs(vcross(d,n))*s->rinmom + s->rmass;
    }
}

/* Overlap(angle - 1) - Overlap(angle + 1) */
int mech_rot_bg_gradient(mech_sprite_t *s)
{
  int o1,o2,oang = s->sprite.anim_p;
  s->sprite.anim_p = (s->sprite.anim_p + 1) & 63;
  o1 = sprite_get_bg_overlap((sprite_t *)s);
  s->sprite.anim_p -= 2;
  if (s->sprite.anim_p < 0)
    s->sprite.anim_p += 64;
  o2 = sprite_get_bg_overlap((sprite_t *)s);
  return o2 - o1;
}

/* The friction is always orthogonal to the normal, so a normal of 0 means that the friction can act in any direction */
void mech_sprite_friction(sprite_t *s1, sprite_t *s2, float *p, float *normal, float amount);

void mech_sprite_collide_old(sprite_t *s1, sprite_t *s2, int x, int y)
{
  float p[2], n[2], v1[2], v2[2];
  float l,dv,I,i1,i2;
  sprite_t *sp;
  int damage;

  p[0] = x;
  p[1] = y;
  mech_point_vel((mech_sprite_t *)s1, p, v1);
  mech_point_vel((mech_sprite_t *)s2, p, v2);
  sprite_get_collision_normal(s1, s2, n);
  l = vabs(n);
  if (l > 0.0001)
    {
      vmul(n,1.0/l);
      /* relative velocity */
      dv = vdot(v2,n) - vdot(v1,n);
      if (dv > 0)
	{
	  /* Calculate impact. There will be problems with two
	     immovable objects (rmass = 0). */
	  I = dv/(mech_eff_rmass((mech_sprite_t *)s1,p,n)+
		  mech_eff_rmass((mech_sprite_t *)s2,p,n));
	  vmul(n,I*1.4); /* elasticity is fixed for now */ 
	  mech_apply_impulse((mech_sprite_t *)s1,p,n);
	  vmul(n,-1);
	  mech_apply_impulse((mech_sprite_t *)s2,p,n);
	  /* Deal with the impact */
	  if (I > level.hard_impact_level) /* hard impact */
	    {
	      sp = sprite_create(&ldust);
	      sprite_set_pos(sp,x,y);
	      n[0] = 0;
	      n[1] = -15;
	      sprite_set_vel(sp,n);
	      sprite_group_insert(effects_group,sp);
	      sound_effect(&sound_mechsmash,x,y);
	      damage = level.hard_impact_damage;
	      sprite_signal(s1,SIGNAL_DAMAGE,&damage);
	      sprite_signal(s2,SIGNAL_DAMAGE,&damage);
	    }
	  else if (I > level.soft_impact_level) /* soft impact */
	    {
	      sp = sprite_create(&sdust);
	      sprite_set_pos(sp,x,y);
	      n[0] = 0;
	      n[1] = -15;
	      sprite_set_vel(sp,n);
	      sprite_group_insert(effects_group,sp);
	      damage = level.soft_impact_damage;
	      sound_effect(&sound_mechbump,x,y);
	      sprite_signal(s1,SIGNAL_DAMAGE,&damage);
	      sprite_signal(s2,SIGNAL_DAMAGE,&damage);
	    }
	}
    }
  if (s1->type->collide)
    s1->type->collide(s1,s2,x,y);
  if (s2->type->collide)
    s2->type->collide(s2,s1,x,y);
}

void mech_sprite_collide(sprite_t *s1, sprite_t *s2, int x, int y)
{
  float n[4], v1[2], v2[2], delta[6];
  float overlap,dot,a,b,c;
  int i;
  
  if (!((((mech_sprite_t *)s1)->mech_flags & MECH_CANROTATE) ||
	(((mech_sprite_t *)s2)->mech_flags & MECH_CANROTATE)))
    {
      mech_sprite_collide_old(s1,s2,x,y);
      return;
    }

  overlap = sprite_get_overlap(s1,s2);

  /* Calculate gradient */
  
  s1->x += 1;
  n[0] = sprite_get_overlap(s1,s2);
  s1->x -= 2;
  n[0] -= sprite_get_overlap(s1,s2);
  s1->x += 1;

  s1->y += 1;
  n[1] = sprite_get_overlap(s1,s2);
  s1->y -= 2;
  n[1] -= sprite_get_overlap(s1,s2);
  s1->y += 1;

  if (((mech_sprite_t *)s1)->mech_flags & MECH_CANROTATE)
    {
      s1->anim_p = (s1->anim_p + 1) & 63;
      n[2] = sprite_get_overlap(s1,s2);
      s1->anim_p = (s1->anim_p + 62) & 63;
      n[2] -= sprite_get_overlap(s1,s2);
      s1->anim_p = (s1->anim_p + 1) & 63;
    }
  else
    {
      n[2] = 0;
    }
  if (((mech_sprite_t *)s2)->mech_flags & MECH_CANROTATE)
    {
      s2->anim_p = (s2->anim_p + 1) & 63;
      n[3] = sprite_get_overlap(s1,s2);
      s2->anim_p = (s2->anim_p + 62) & 63;
      n[3] -= sprite_get_overlap(s1,s2);
      s2->anim_p = (s2->anim_p + 1) & 63;
    }
  else 
    {
      n[3] = 0;
    }

  for (i=0;i<4;i++)
    n[i] *= 0.5;

  c = ((n[0]*n[0] + n[1]*n[1])*
       (((mech_sprite_t *)s1)->rmass + ((mech_sprite_t *)s2)->rmass))
    + (n[2]*n[2]*((mech_sprite_t *)s1)->rinmom + 
       n[3]*n[3]*((mech_sprite_t *)s1)->rinmom);

  if (c < 0.1)
    {
      /* We could do some friction here */
      return;
    }
  c = -overlap/c;
  
  /* Delta determines how easy it is to change a 
     certain coordinate */

  delta[0] = c*n[0]*((mech_sprite_t *)s1)->rmass;
  delta[1] = c*n[1]*((mech_sprite_t *)s1)->rmass;
  delta[2] = c*n[2]*((mech_sprite_t *)s1)->rinmom;
  delta[3] = -c*n[0]*((mech_sprite_t *)s2)->rmass;
  delta[4] = -c*n[1]*((mech_sprite_t *)s2)->rmass;
  delta[5] = c*n[3]*((mech_sprite_t *)s2)->rinmom;

  /* Separate the sprites. The factor is experimental.
     Add a maximum separation for stability. */

  vmadd(s1->pos,1.0,delta);
  vmadd(s2->pos,1.0,delta+3);
  
  /* The rotation gradient is probably not 
     as good as the linear, so add a maximum
     adjustment here */
  if ((delta[2] < 10) && (delta[2] > -10))
    ((mech_sprite_t *)s1)->angle += delta[2];
  else 
    ((mech_sprite_t *)s1)->angle += sgn(delta[2])*10;
 
  if ((delta[5] < 10) && (delta[5] > -10))
    ((mech_sprite_t *)s2)->angle += delta[5];
  else 
    ((mech_sprite_t *)s2)->angle += sgn(delta[5])*10;


  /* Adjust velocities */

  a = 
    n[0]*(s1->vel[0] - s2->vel[0]) + 
    n[1]*(s1->vel[1] - s2->vel[1]) + 
    n[2]*((mech_sprite_t *)s1)->ang_vel +
    n[3]*((mech_sprite_t *)s2)->ang_vel;

  if (a > 0)
    {
      b = n[0]*delta[0] + n[1]*delta[1] + n[2]*delta[2] 
	- n[0]*delta[3] - n[1]*delta[4] + n[3]*delta[5];
      
      c = -1.4*a/b;
      
      s1->vel[0] += c*delta[0];
      s1->vel[1] += c*delta[1];
      ((mech_sprite_t *)s1)->ang_vel += c*delta[2];
      s2->vel[0] += c*delta[3];
      s2->vel[1] += c*delta[4];
      ((mech_sprite_t *)s2)->ang_vel += c*delta[5];
    }

  if (s1->type->collide)
    s1->type->collide(s1,s2,x,y);
  if (s2->type->collide)
    s2->type->collide(s2,s1,x,y);
}

void mech_sprite_bg_collide_old(sprite_t *s, int x, int y)
{
  float p[2], n[2], v[2];
  float l,dv;
  sprite_t *sp;
  int damage;

  if (((mech_sprite_t *)s)->mech_flags & MECH_INBG)
    return;

  p[0] = x;
  p[1] = y;
  sprite_remove_from_bg(s,n);
  l = vabs(n);
  if (l > 0.0001)
    {
      vmul(n,1.0/l);
      dv = -vdot(s->vel,n);
      if (dv < 0)
	{
	  dv = dv/mech_eff_rmass((mech_sprite_t *)s, p, n);
	  vmul(n,dv*1.35); 
	  vadd(((mech_sprite_t *)s)->lin_impulse,n);
#if 0
	  if (dv < -level.hard_impact_level) /* hard impact */
	    {
	      sp = sprite_create(&ldust);
	      sprite_set_pos(sp,x,y);
	      n[0] = 0;
	      n[1] = -15;
	      sprite_set_vel(sp,n);
	      sprite_group_insert(effects_group,sp);
	      damage = level.hard_impact_damage;
	      sound_effect(&sound_mechgroundsmash,x,y);
	      sprite_signal(s,SIGNAL_DAMAGE,&damage);
	    }
	  else if (dv < -level.soft_impact_level) /* soft impact */
	    {
	      sp = sprite_create(&sdust);
	      sprite_set_pos(sp,x,y);
	      n[0] = 0;
	      n[1] = -15;
	      sprite_set_vel(sp,n);
	      sprite_group_insert(effects_group,sp);
	      damage = level.soft_impact_damage;
	      sound_effect(&sound_mechgroundbump,x,y);
	      sprite_signal(s,SIGNAL_DAMAGE,&damage);
	    }
#endif
	}
    }
  if (s->type->bg_collide)
    s->type->bg_collide(s,x,y);
}


void mech_sprite_bg_collide(sprite_t *s1, int x, int y)
{
  float n[3], v1[2], delta[3];
  float overlap,dot,a,b,c;
  int i;
  
  if (!(((mech_sprite_t *)s1)->mech_flags & MECH_CANROTATE))
    {
      mech_sprite_bg_collide_old(s1,x,y);
      return;
    }

  overlap = sprite_get_bg_overlap(s1);

  /* Calculate gradient */
  
  s1->x += 1;
  n[0] = sprite_get_bg_overlap(s1);
  s1->x -= 2;
  n[0] -= sprite_get_bg_overlap(s1);
  s1->x += 1;

  s1->y += 1;
  n[1] = sprite_get_bg_overlap(s1);
  s1->y -= 2;
  n[1] -= sprite_get_bg_overlap(s1);
  s1->y += 1;

  s1->anim_p = (s1->anim_p + 1) & 63;
  n[2] = sprite_get_bg_overlap(s1);
  s1->anim_p = (s1->anim_p + 62) & 63;
  n[2] -= sprite_get_bg_overlap(s1);
  s1->anim_p = (s1->anim_p + 1) & 63;

  for (i=0;i<3;i++)
    n[i] *= 0.5;

  c = (n[0]*n[0] + n[1]*n[1])*((mech_sprite_t *)s1)->rmass + 
    n[2]*n[2]*((mech_sprite_t *)s1)->rinmom;
     
  if (c < 0.1)
    {
      /* We could do some friction here */
      return;
    }
  c = -overlap/c;
  
  /* Delta is the 'normal', in the space of the
     six degrees of freedom x1,y1,ang1,x2,y2,ang2.
     It determines how much the coordinates should be 
     adjusted to remove the overlap */
  
  delta[0] = c*n[0]*((mech_sprite_t *)s1)->rmass;
  delta[1] = c*n[1]*((mech_sprite_t *)s1)->rmass;
  delta[2] = c*n[2]*((mech_sprite_t *)s1)->rinmom;
  
  /* Separate the sprites. The factor is experimental.
     Add a maximum separation for stability. */

  vmadd(s1->pos,1.0,delta);
  ((mech_sprite_t *)s1)->angle += delta[2];
  
  /* Adjust velocities */

  a = n[0]*s1->vel[0] + n[1]*s1->vel[1] + 
    n[2]*((mech_sprite_t *)s1)->ang_vel;

  if (a > 0)
    {
      b = n[0]*delta[0] + n[1]*delta[1] + n[2]*delta[2];
      
      c = -1.2*a/b;
      
      s1->vel[0] += c*delta[0];
      s1->vel[1] += c*delta[1];
      ((mech_sprite_t *)s1)->ang_vel += c*delta[2];
    }

  if (s1->type->bg_collide)
    s1->type->bg_collide(s1,x,y);
}


void mech_boundary(mech_sprite_t *ms)
{
  sprite_t *s = (sprite_t *)ms;
  float v[2];

  if (s->x < level.xmin) 
    {
      sprite_set_pos(s,s->x + level.xmax,s->y);
      sprite_signal(s,SIGNAL_LEVELWARP,0);
    }
  if (s->y < level.ymin) 
    {
      sprite_get_vel(s,v);
      if (v[1] < 0) 
	v[1] = 0;
      sprite_set_vel(s,v);
    }
  if (s->x > level.xmax)
    {
      sprite_set_pos(s,s->x - level.xmax, s->y);
      sprite_signal(s,SIGNAL_LEVELWARP,0);
    }
  if (s->y > level.ymax)
    {
      sprite_get_vel(s,v);
      if (v[1] > 0) 
	v[1] = 0;
      sprite_set_vel(s,v);
    }
}
