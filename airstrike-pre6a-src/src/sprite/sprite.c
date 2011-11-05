#include <malloc.h>
#include <assert.h>
#include "sprite.h"
#include "refcount.h"
#include "bitmask.h"
#include "utils.h"

sprite_t *sprite_create(sprite_type_t *type)
{
  sprite_t *s;
  if (!type->_setup_ok)
    {
      fprintf(stderr,"Loading sprite type '%s'\n",type->name);
      CRITICAL(type->setup() == 0);
      type->_setup_ok = 1;
    }
  s = type->create_sprite();
  s->type = type;
  sprite_set_pos(s,0,0);
  return s;
}

void sprite_set_animation(sprite_t *s, animation_t *anim)
{
  s->animation = anim;
}

void sprite_set_pos(struct sprite *s, int x, int y)
{
  s->x = x;
  s->y = y;
  s->pos[0] = x;
  s->pos[1] = y;
}

void sprite_set_vel(sprite_t *s, const float v[])
{
  s->vel[0] = v[0]/32;
  s->vel[1] = v[1]/32;
}

void sprite_get_vel(sprite_t *s, float v[])
{
  v[0] = s->vel[0]*32;
  v[1] = s->vel[1]*32;
}

void sprite_get_collision_normal(sprite_t *s1, sprite_t *s2, float n[])
{
  n[0] = 
    bitmask_overlap_area(s1->animation->masks[s1->anim_p],
			 s2->animation->masks[s2->anim_p],
			 s2->x - s2->animation->xoff - s1->x 
			 + s1->animation->xoff + 1, 
			 s2->y - s2->animation->yoff - s1->y
			 + s1->animation->yoff) -
    bitmask_overlap_area(s1->animation->masks[s1->anim_p],
			 s2->animation->masks[s2->anim_p],
			 s2->x - s2->animation->xoff - s1->x 
			 + s1->animation->xoff - 1, 
			 s2->y - s2->animation->yoff - s1->y
			 + s1->animation->yoff);
  n[1] = 
    bitmask_overlap_area(s1->animation->masks[s1->anim_p],
			 s2->animation->masks[s2->anim_p],
			 s2->x - s2->animation->xoff - s1->x 
			 + s1->animation->xoff, 
			 s2->y - s2->animation->yoff - s1->y
			 + s1->animation->yoff + 1) -
    bitmask_overlap_area(s1->animation->masks[s1->anim_p],
			 s2->animation->masks[s2->anim_p],
			 s2->x - s2->animation->xoff - s1->x 
			 + s1->animation->xoff, 
			 s2->y - s2->animation->yoff - s1->y
			 + s1->animation->yoff - 1);
}

int sprite_get_overlap(sprite_t *s1, sprite_t *s2)
{
  return bitmask_overlap_area(s1->animation->masks[s1->anim_p],
			      s2->animation->masks[s2->anim_p],
			      s2->x - s2->animation->xoff 
			      - s1->x + s1->animation->xoff, 
			      s2->y - s2->animation->yoff 
			      - s1->y + s1->animation->yoff);
}

int sprite_get_bg_overlap(sprite_t *s)
{
  return bitmask_overlap_area(sprite_global.bg_mask,
			      s->animation->masks[s->anim_p],
			      s->x - s->animation->xoff, 
			      s->y - s->animation->yoff);
}

void sprite_get_bg_collision_normal(sprite_t *s, float n[])
{
  n[0] = 
    bitmask_overlap_area(sprite_global.bg_mask,
			 s->animation->masks[s->anim_p],
			 s->x - s->animation->xoff + 1, 
			 s->y - s->animation->yoff) -
    bitmask_overlap_area(sprite_global.bg_mask,
			 s->animation->masks[s->anim_p],
			 s->x - s->animation->xoff - 1, 
			 s->y - s->animation->yoff);

  n[1] = 
    bitmask_overlap_area(sprite_global.bg_mask,
			 s->animation->masks[s->anim_p],
			 s->x - s->animation->xoff, 
			 s->y - s->animation->yoff + 1) -
    bitmask_overlap_area(sprite_global.bg_mask,
			 s->animation->masks[s->anim_p],
			 s->x - s->animation->xoff, 
			 s->y - s->animation->yoff - 1);
}

int sprite_remove_from_bg(sprite_t *s, float n[])
{
  int dx,dy,a,da2;
  float t;
  a = bitmask_overlap_area(sprite_global.bg_mask,
			   s->animation->masks[s->anim_p],
			   s->x - s->animation->xoff, 
			   s->y - s->animation->yoff);
  if (a == 0) return 0;
  dx =
    bitmask_overlap_area(sprite_global.bg_mask,
			 s->animation->masks[s->anim_p],
			 s->x - s->animation->xoff + 1, 
			 s->y - s->animation->yoff) -
    bitmask_overlap_area(sprite_global.bg_mask,
			 s->animation->masks[s->anim_p],
			 s->x - s->animation->xoff - 1, 
			 s->y - s->animation->yoff);
  dy = 
    bitmask_overlap_area(sprite_global.bg_mask,
			 s->animation->masks[s->anim_p],
			 s->x - s->animation->xoff, 
			 s->y - s->animation->yoff + 1) -
    bitmask_overlap_area(sprite_global.bg_mask,
			 s->animation->masks[s->anim_p],
			 s->x - s->animation->xoff, 
			 s->y - s->animation->yoff - 1);
  n[0] = dx;
  n[1] = dy;
  da2 = dx*dx + dy*dy;
  if (da2 == 0)
    return -a;
  t = -0.5*a/(float)da2;
  s->pos[0] += dx*t;
  s->pos[1] += dy*t;
  return a;
}

static INLINE void sprite_deref(struct sprite *s)
{
  DEREF(s,s->type->free_sprite);
}

int sprite_alive(sprite_t *s)
{
  return !(REFDEAD(s));
}

int sprite_aquire(sprite_t *src, sprite_t **dest)
{
  if ((src) && !REFDEAD(src) && (*dest == 0))
    {
      *dest = src;
      REF(*dest);
      return 1;
    }
  else return 0;
}

int sprite_isvalid(sprite_t **ref)
{
  if (*ref == 0)
    {
      return 0;
    }
  else if (REFDEAD(*ref))
    {
      DEREF(*ref,(*ref)->type->free_sprite);
      *ref = 0;
      return 0;
    }
  else
    return 1;
}

void sprite_release(sprite_t **ref)
{
  if (*ref)
    {
      DEREF(*ref,(*ref)->type->free_sprite);
      *ref = 0;
    }
}

void sprite_kill(sprite_t *s)
{
  if (s)
    REFKILL(s);
}

void sprite_signal(sprite_t *s, int signal, void *data)
{
  if (s->type->signal)
    s->type->signal(s,signal,data);
}

/* Frame checklist
 * Sprite time propagation dt to new frame:
 * 1) Move all sprites according to velocity
 * 2) Advance animation as of dt, execute callbacks as
 *    frames are entered. Do each sprite completely 
 *    before going to the next.
 * 3) Find collisions, execute callbacks when encountered
 * 4) Execute frame update functions (i.e. react to step 1..3).
 * 5) Clean up dead sprites
 * 6) Draw remaining sprites (may be combined w 5)
 */

static void sprite_move(sprite_t *s, unsigned int ticks)
{
  s->pos[0] += s->vel[0];
  s->pos[1] += s->vel[1];
  s->x = (int)(s->pos[0] + 0.5);
  s->y = (int)(s->pos[1] + 0.5);
}

static void sprite_animate(sprite_t *s, unsigned int ticks)
{
  /* advance animation */
  s->_t_off += ticks;
  while (s->_t_off > s->animation->delay)
    {
      s->_t_off -= s->animation->delay;
      /* Start anew if there is no next frame */
      if (s->animation->next_frame)
	s->animation = s->animation->next_frame;
      else
	s->animation = s->animation->base_frame;

      if (s->animation->trigger)
	s->animation->trigger(s);
    }
}

sprite_group_t *sprite_group_create(void)
{
  sprite_group_t *l;
  l = malloc(sizeof(sprite_group_t));
  l->nr_sprites = 0;
  l->max_sprites = 32; /* Start with an initial max of 32 */
  l->sprites = malloc(sizeof(sprite_t *)*(l->max_sprites));
  return l;
}

void sprite_group_free(sprite_group_t *group)
{
  int i;
  for (i=0; i<group->nr_sprites; i++)
    {
      sprite_deref(group->sprites[i]);
    }
  free(group->sprites);
  free(group);
}

void sprite_group_insert(sprite_group_t *group, sprite_t *s)
{
  if (group->nr_sprites >= group->max_sprites)
    {
      CRITICAL(group->sprites = realloc(group->sprites,2*group->max_sprites*sizeof(sprite_t *)));
      group->max_sprites *= 2;
    }
  group->sprites[group->nr_sprites++] = s;
  REF(s);
}

/* Remove dead sprites from the list */
void sprite_group_cleanup(sprite_group_t *group)
{
  sprite_t **write;
  int i,n;
  
  n = group->nr_sprites;
  for (i=0;(i<n) && (sprite_alive(group->sprites[i]));i++)
    {}
  write = group->sprites + i;
  for (;i<n;i++)
    {
      if (!sprite_alive(group->sprites[i]))
	{
	  sprite_deref(group->sprites[i]);
	  group->nr_sprites--;
	}
      else
	{
	  *write = group->sprites[i];
	  write++;
	}
    }
}

void sprite_group_remove(sprite_group_t *group, sprite_t *s)
{
  int i,j,n;
  
  n = group->nr_sprites;
  for (i=0; i<n ;i++)
    {
      if (group->sprites[i] == s)
	{
	  sprite_deref(s);
	  for (j=i;j<n-1;j++)
	    group->sprites[j] = group->sprites[j+1];
	  group->nr_sprites--;
	  return;
	}
    }
}

void sprite_group_coll(sprite_group_t *group,
		       void (*f)(struct sprite *this_sprite, 
				 struct sprite *other_sprite, 
				 int x, int y))
{
  int i,j,x,y;
  sprite_t *s1,*s2;
  bitmask *b1;

  if (f)
    {
      for (i = 0; i < group->nr_sprites - 1; i++)
	{
	  s1 = group->sprites[i];
	  b1 = s1->animation->masks[s1->anim_p];
	  for (j = i + 1; j < group->nr_sprites; j++)
	    {
	      s2 = group->sprites[j];
	      if (bitmask_overlap_pos(b1,s2->animation->masks[s2->anim_p],
				      s2->x - s2->animation->xoff - 
				      s1->x + s1->animation->xoff, 
				      s2->y - s2->animation->yoff - 
				      s1->y + s1->animation->yoff, 
				      &x, &y))
		{
		  x += s1->x - s1->animation->xoff;
		  y += s1->y - s1->animation->yoff;
		  f(s1,s2,x,y);
		}
	    }
	}
    }
  else
    {
      for (i = 0; i < group->nr_sprites - 1; i++)
	{
	  s1 = group->sprites[i];
	  b1 = s1->animation->masks[s1->anim_p];
	  for (j = i + 1; j < group->nr_sprites; j++)
	    {
	      s2 = group->sprites[j];
	      if (bitmask_overlap_pos(b1,s2->animation->masks[s2->anim_p],
				      s2->x - s2->animation->xoff - 
				      s1->x + s1->animation->xoff, 
				      s2->y - s2->animation->yoff - 
				      s1->y + s1->animation->yoff, 
				      &x, &y))
		{
		  x += s1->x - s1->animation->xoff;
		  y += s1->y - s1->animation->yoff;
		  if (s1->type->collide)
		    s1->type->collide(s1,s2,x,y);
		  if (s2->type->collide)
		    s2->type->collide(s2,s1,x,y);
		}
	    }
	}
    }
}

void sprite_group_coll2(sprite_group_t *group1, 
			sprite_group_t *group2,
			void (*f)(struct sprite *this_sprite, 
				  struct sprite *other_sprite, 
				  int x, int y))
{
  int i,j,x,y;
  sprite_t *s1,*s2;
  bitmask *b1;
  if (f)
    {
      for (i = 0; i < group1->nr_sprites; i++)
	{
	  s1 = group1->sprites[i];
	  b1 = s1->animation->masks[s1->anim_p];
	  for (j = 0; j < group2->nr_sprites; j++)
	    {
	      s2 = group2->sprites[j];
	      if (bitmask_overlap_pos(b1,s2->animation->masks[s2->anim_p],
				      s2->x - s2->animation->xoff - 
				      s1->x + s1->animation->xoff, 
				      s2->y - s2->animation->yoff - 
				      s1->y + s1->animation->yoff, 
				      &x, &y))
		{
		  x += s1->x - s1->animation->xoff;
		  y += s1->y - s1->animation->yoff;
		  f(s1,s2,x,y);
		}
	    }
	}
    }
  else
    {
      for (i = 0; i < group1->nr_sprites; i++)
	{
	  s1 = group1->sprites[i];
	  b1 = s1->animation->masks[s1->anim_p];
	  for (j = 0; j < group2->nr_sprites; j++)
	    {
	      s2 = group2->sprites[j];
	      if (bitmask_overlap_pos(b1,s2->animation->masks[s2->anim_p],
				      s2->x - s2->animation->xoff - 
				      s1->x + s1->animation->xoff, 
				      s2->y - s2->animation->yoff - 
				      s1->y + s1->animation->yoff, 
				      &x, &y))
		{
		  x += s1->x - s1->animation->xoff;
		  y += s1->y - s1->animation->yoff;
		  if (s1->type->collide)
		    s1->type->collide(s1,s2,x,y);
		  if (s2->type->collide)
		    s2->type->collide(s2,s1,x,y);
		}
	    }
	}
    }
}

void sprite_group_bg_coll(sprite_group_t *group,
			  void (*f)(struct sprite *sprite, int x, int y))
{
  int i,x,y;
  sprite_t *s1;


  if (f)
    {
      for (i = 0; i < group->nr_sprites; i++)
	{
	  s1 = group->sprites[i];
	  if (bitmask_overlap_pos(sprite_global.bg_mask,
				  s1->animation->masks[s1->anim_p],
				  s1->x - s1->animation->xoff,
				  s1->y - s1->animation->yoff, 
				  &x, &y))
	    {
	      f(s1,x,y);
	    }
	}
    }
  else
    {
        for (i = 0; i < group->nr_sprites; i++)
	{
	  s1 = group->sprites[i];
	  if (bitmask_overlap_pos(sprite_global.bg_mask,
				  s1->animation->masks[s1->anim_p],
				  s1->x - s1->animation->xoff,
				  s1->y - s1->animation->yoff, 
			      &x, &y))
	    {
	      if (s1->type->bg_collide)
		s1->type->bg_collide(s1,x,y);
	    }
	}
    }
}

void sprite_group_point_coll(sprite_group_t *group,
			     int point_x,int point_y,
			     void (*f)(struct sprite *sprite, 
				       int x, int y))
{
  int i,lx,ly;
  sprite_t *s;
  bitmask *m;

  for (i = 0; i < group->nr_sprites; i++)
    {
      s = group->sprites[i];
      m = s->animation->masks[s->anim_p];
      lx = point_x - s->x + s->animation->xoff;
      ly = point_y - s->y + s->animation->yoff;
      if ((lx >= 0) && (lx < m->w) && (ly >= 0) && (ly < m->h) &&
	  (bitmask_getbit(m,lx,ly)))
	f(s,point_x,point_y);
    }
}

void sprite_group_move(sprite_group_t *group, unsigned int ms)
{
  int i;
  for (i=0; i < group->nr_sprites; i++)
    sprite_move(group->sprites[i],ms);
}

void sprite_group_pos_update(sprite_group_t *group)
{
  int i;
  for (i=0; i < group->nr_sprites; i++)
    {
      group->sprites[i]->x = group->sprites[i]->pos[0] + 0.5;
      group->sprites[i]->y = group->sprites[i]->pos[1] + 0.5;
    }
}

void sprite_group_animate(sprite_group_t *group, unsigned int delta)
{
  int i;
  for (i=0; i < group->nr_sprites; i++)
    sprite_animate(group->sprites[i],delta);
}

void sprite_group_update(sprite_group_t *group)
{
  int i;
  for (i=0; i < group->nr_sprites; i++)
    if (group->sprites[i]->type->update)
      group->sprites[i]->type->update(group->sprites[i]);
}

void sprite_group_draw(sprite_group_t *group)
{
  int i;
  for (i=0; i < group->nr_sprites; i++)
    {
      animation_draw(group->sprites[i]->animation,
		     group->sprites[i]->x,
		     group->sprites[i]->y,
		     group->sprites[i]->anim_p);
    }
}




