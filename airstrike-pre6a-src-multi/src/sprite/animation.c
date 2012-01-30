#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL/SDL_image.h>
#include "sprite.h"
#include "bitmask.h"

void animation_draw(struct animation *a, int x, int y, int p)
{
  SDL_Rect r;
  assert(p < a->nr_pframes);
  r.x = x - sprite_global.xoff - a->xoff;
  r.y = y - sprite_global.yoff - a->yoff;
  r.w = a->images[p]->w;
  r.h = a->images[p]->h;
  SDL_BlitSurface(a->images[p],0,sprite_global.display,&r);
  sprite_dirty(&r);
}

void animation_make_loop(animation_t *anim)
{
  animation_last_frame(anim)->next_frame = anim;
}

animation_t *animation_last_frame(animation_t *anim)
{
  animation_t *a = anim;
  while ((a->next_frame != 0) && (a->next_frame != anim)) 
    a = a->next_frame;
  return a;
}

animation_t *animation_frame_n(animation_t *anim, int frame_nr)
{
  while (frame_nr--)
    {
      if (!anim) return 0;
      anim = anim->next_frame;
    }
  return anim;
}

animation_t *animation_load(const char *image, int nr_pframes, 
			    int nr_tframes, unsigned int delay)
{
  int i,j;
  animation_t *first = 0,*f,*prev = 0,*base = 0;
  SDL_Surface *source;
  SDL_Rect r;

  assert(nr_pframes > 0);
  assert(nr_tframes > 0);
  source = IMG_Load(image);
  if (!source)
    {
      fprintf(stderr, "animation_load(): Error opening image file '%s'. Quitting!\n",image);
      exit(1);
    }
  r.h = source->h;
  r.w = source->w/(nr_pframes*nr_tframes);
  r.y = 0;
  for (i=0;i<nr_tframes;i++)
    {
      f = malloc(sizeof(animation_t));
      assert(f);
      f->images = malloc(sizeof(SDL_Surface *)*nr_pframes);
      assert(f->images);
      f->masks = malloc(sizeof(bitmask *)*nr_pframes);
      assert(f->masks);
      if (i==0)
	base = f;
      f->base_frame = base;
      f->xoff = r.w/2;
      f->yoff = r.h/2;
      f->trigger = 0;
      f->next_frame = 0;
      f->nr_pframes = nr_pframes;
      f->delay = delay;
      if (!first)
	first = f;
      if (prev)
	prev->next_frame = f;
      /* Convert to display format and create the individual frames*/
      for (j=0;j<nr_pframes;j++)
	{
	  r.x = (i + j*nr_tframes) * r.w;
	  if (sprite_global.opt_alpha < 2)
	    GetSubSurface(source,&r,&(f->images[j]),&(f->masks[j]),
			  128,128,SDL_RLEACCEL,0);
	  else
	    GetSubSurface(source,&r,&(f->images[j]),&(f->masks[j]),
			  0,128,SDL_RLEACCEL,0);
	}
      prev = f;
    }
  SDL_FreeSurface(source);
  return first;
}

void animation_set_alpha(animation_t *a, unsigned char alpha)
{
  int i;
  animation_t *anim = a;

  if (sprite_global.opt_alpha == 1)
    {
      for (i=0;i<a->nr_pframes;i++)
	{	  
	  SDL_SetAlpha(a->images[i],SDL_SRCALPHA,alpha);
	}
      while (a->next_frame && (a->next_frame != anim)) 
	{
	  a = a->next_frame;
	  for (i=0;i<a->nr_pframes;i++)
	    {	  
	      SDL_SetAlpha(a->images[i],SDL_SRCALPHA,alpha);
	    }
	}
    }
}

void animation_set_center(struct animation *a, int x, int y)
{
  animation_t *anim = a;
  a->xoff = x;
  a->yoff = y;
  while (a->next_frame && (a->next_frame != anim)) 
    {
      a = a->next_frame;
      a->xoff = x;
      a->yoff = y;
    }
}
