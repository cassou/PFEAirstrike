#include <assert.h>
#include <malloc.h>
#include "sprite.h"
#include "bitmask.h"
#include "refcount.h"
#include "dirty.h"

struct sprite_global sprite_global = 
{
  /* .display */ 0,
  /* .xoff = */ 0,
  /*.yoff =*/ 0,
  /*.bg_image = */0,
  /*.bg_mask =*/ 0,
  /*.dt =*/ 30,
  /*.display_dt = */30,
  /*.frame_start =*/ 0,
  /*.game_clock =*/ 0,
  /*.opt_alpha =*/ 2,
};

static int in_frame = 0; /* this is 1 if we are inside a frame */


void sprite_timer_set(sprite_timer_t *timer, unsigned int delay)
{
  *timer = delay + sprite_global.game_clock;
}

int sprite_timer_finished(sprite_timer_t timer)
{
  return timer <= sprite_global.game_clock;
}

static int nr_alarms = 0;
static int max_alarms = 32; /* initial max */
static struct alarm
{
  sprite_t *target;
  int signal;
  void *data;
  unsigned int at;
} *alarms;

void sprite_alarm(unsigned int delay, sprite_t *target, int signal, void *data)
{
  if (sprite_alive(target))
    {
      if (nr_alarms >= max_alarms)
	{
	  max_alarms*=2;
	  alarms = realloc(alarms,max_alarms*sizeof(*alarms));
	  assert(alarms);
	}
      alarms[nr_alarms].at = sprite_global.game_clock + delay;
      alarms[nr_alarms].target = target;
      alarms[nr_alarms].signal = signal;
      alarms[nr_alarms].data = data;
      REF(target);
      nr_alarms++;
    }
  else
    {
      fprintf(stderr,"Warning: Setting alarm to dead sprite.\n");
    }
}

void sprite_start_frame()
{
  int i;
  in_frame = 1;
  sprite_global.frame_start = SDL_GetTicks();
  sprite_global.game_clock += sprite_global.dt;
  dirty_restore(sprite_global.display, sprite_global.bg_image,
		sprite_global.xoff, sprite_global.yoff);
  for (i=0;i<nr_alarms;i++)
    {
      if (!sprite_alive(alarms[i].target))
	{
	  DEREF(alarms[i].target,alarms[i].target->type->free_sprite);
	  alarms[i--] = alarms[--nr_alarms];
	}
      else if (alarms[i].at <= sprite_global.game_clock)
	{
	  sprite_signal(alarms[i].target,alarms[i].signal,alarms[i].data);
	  DEREF(alarms[i].target,alarms[i].target->type->free_sprite);
	  alarms[i--] = alarms[--nr_alarms];
	}
    }
}

static void sprite_dirty_update();

unsigned int sprite_end_frame()
{
  unsigned int dt;  
  dirty_update(sprite_global.display);
  in_frame = 0;
  dt = SDL_GetTicks() - sprite_global.frame_start;
  if (dt < sprite_global.display_dt)
    {
      SDL_Delay(sprite_global.display_dt - dt);
      /* for some reason SDL_Delay might sleep up to 10 ms to little,
	 and hence the stuff below (Linux 2.4.18, debian) */
      dt = SDL_GetTicks() - sprite_global.frame_start;
      if (dt < sprite_global.display_dt - 5)
	{
	  SDL_Delay(sprite_global.display_dt - dt);
	}
      return sprite_global.display_dt - dt;
    }
  else return 0;
}

void sprite_background_resize(int xsize, int ysize)
{
  SDL_Surface *surf;

  if (sprite_global.bg_image)
    sprite_background_free();
  sprite_global.bg_mask = bitmask_create(xsize,ysize);
  surf = SDL_CreateRGBSurface(SDL_SWSURFACE,xsize,ysize,
			      32,0xff000000,0xff0000,0xff00,0x0);
  assert(surf);
  sprite_global.bg_image = SDL_DisplayFormat(surf);
  SDL_FreeSurface(surf);
}

void sprite_background_free()
{
  SDL_FreeSurface(sprite_global.bg_image);
  bitmask_free(sprite_global.bg_mask);
  sprite_global.bg_image = 0;
  sprite_global.bg_mask = 0;
}

void sprite_background_compose(sprite_t *brush, int use_mask)
{
  SDL_Rect r;
  r.x = brush->x - brush->animation->xoff;
  r.y = brush->y - brush->animation->yoff;
  r.w = brush->animation->images[brush->anim_p]->w;
  r.h = brush->animation->images[brush->anim_p]->h;
  SDL_BlitSurface(brush->animation->images[brush->anim_p],0,
		  sprite_global.bg_image,&r);
  if (use_mask)
    {
      bitmask_draw(sprite_global.bg_mask, 
		   brush->animation->masks[brush->anim_p],
		   brush->x - brush->animation->xoff,
		   brush->y - brush->animation->yoff);
    }
}

void sprite_viewport_center_on(sprite_t *s)
{
  int px,py,dx,dy;
  static int thres = 1;
  if (!s)
    return;
  px = s->x - sprite_global.display->w/2;
  py = s->y - sprite_global.display->h/2;

  if (px < 0)
    px = 0;
  else if (px >= sprite_global.bg_image->w - sprite_global.display->w)
    px = sprite_global.bg_image->w - sprite_global.display->w - 1;
  
  if (py < 0)
    py = 0;
  else if (py >= sprite_global.bg_image->h - sprite_global.display->h)
    py = sprite_global.bg_image->h - sprite_global.display->h - 1;

  dx = px - sprite_global.xoff;
  dy = py - sprite_global.yoff;

  if (dx*dy + dy*dy > thres)
    {
      sprite_global.xoff += dx/2;
      sprite_global.yoff += dy/2;	
      sprite_dirty_all();
    }
}

void sprite_viewport_center(int x, int y)
{
  int px,py;
  px = x - sprite_global.display->w/2;
  py = y - sprite_global.display->h/2;

  if (px < 0)
    px = 0;
  else if (px >= sprite_global.bg_image->w - sprite_global.display->w)
    px = sprite_global.bg_image->w - sprite_global.display->w - 1;
  
  if (py < 0)
    py = 0;
  else if (py >= sprite_global.bg_image->h - sprite_global.display->h)
    py = sprite_global.bg_image->h - sprite_global.display->h - 1;

  if (sprite_global.xoff != px || sprite_global.yoff != py)
    {
      sprite_global.xoff = px;
      sprite_global.yoff = py;
      sprite_dirty_all();
    }
}

void sprite_setup()
{
  alarms = malloc(sizeof(*alarms)*max_alarms);
  dirty_setup(sprite_global.display->w,sprite_global.display->h);
}

void sprite_dirty(SDL_Rect *r)
{
  dirty_rect(r);
} 

void sprite_dirty_all(void)
{
  dirty_all();
}

void sprite_bg_dirty(SDL_Rect *r)
{
  dirty_rect_bg(r);
}

void sprite_bg_dirty_all(void)
{
  dirty_all_bg();
}
