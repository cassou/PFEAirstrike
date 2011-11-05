#include <assert.h>
#include <malloc.h>
#include <SDL/SDL_image.h>
#include "text.h"
#include "sprite.h"
#include "config.h"

static SDL_Surface *chars[256];

font_t *big_font = 0;
font_t *small_font = 0;

int text_setup(void)
{
  static char *ascii = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~ ";
  big_font = text_load_font(path_to_data("bigfont.png"),ascii,15,25);
  small_font = text_load_font(path_to_data("smallfont-green.png"),ascii,9,11);
  return 0;
}

font_t *text_load_font(const char *image, const char *chars, 
		       int char_width, int char_pitch)
{
  SDL_Surface *src;
  SDL_Rect r;
  char *c;
  font_t *font = calloc(1,sizeof(*font));

  assert(font);
  src = IMG_Load(image);
  assert(src);
  font->char_w = char_width;
  font->char_h = src->h;
  font->char_d = char_pitch;
  r.x = 0;
  r.y = 0;
  r.w = font->char_w;
  r.h = font->char_h;
  for (;*chars;chars++)
    {
      GetSubSurface(src,&r,&(font->chars[(unsigned char)*chars]),0,
		    0,0,SDL_RLEACCEL,0);
      r.x += font->char_d;
    }
  return font;
}


void text_render(SDL_Surface *target, SDL_Rect *r,
		 font_t *font,
		 int x, int y, 
		 enum text_alignment xalign, 
		 enum text_alignment yalign,
		 char *s)
{
  int w = 0,h = font->char_h; 
  int rw = 0;
  int xstart,ystart;
  char *c = s;
  SDL_Rect t;
  /* How big rect we are going to cover? */
  while (*c)
    {
      if (*c == '\n')
	{
	  if (rw > w)
	    w = rw;
	  rw = 0;
	  if (*(c+1))
	    h += font->char_h;
	}
      else
	rw += font->char_w;
      c++;
    }
  if (rw > w)
    w = rw;

  /* Where to start drawing? */
  if (xalign == ALIGN_LEFT)
    xstart = x;
  else if (xalign == ALIGN_RIGHT)
    xstart = x - w;
  else
    xstart = x - w/2;

  if (yalign == ALIGN_TOP)
    ystart = y;
  else if (yalign == ALIGN_BOTTOM)
    ystart = y - h;
  else
    ystart = y - h/2;

  if (r)
    {
      r->x = xstart;
      r->y = ystart;
      r->w = w;
      r->h = h;
    }
  t.x = xstart;
  t.y = ystart;
  t.w = font->char_w;
  t.h = font->char_h;
  /* Now we can finally render */  
  for (c = s; *c != 0; c++)
    {
      if (*c == '\n')
	{
	  t.x = xstart;
	  t.y += font->char_h;
	}
      else if (font->chars[(unsigned char)*c])
	{
	  SDL_BlitSurface(font->chars[(unsigned char)*c],0,target,&t);
	  t.x += font->char_w;
	}
      else
	{
	  t.x += font->char_w;
	}
    }
  /* clip and mark dirty */
  if (xstart + w > target->w)
    w = target->w - xstart;
  if (ystart + h > target->h)
    h = target->h - ystart;
  if (xstart < 0)
    xstart = 0;
  if (ystart < 0)
    ystart = 0;
  t.x = xstart;
  t.y = ystart;
  t.w = w;
  t.h = h;
  sprite_dirty(&t);
}

