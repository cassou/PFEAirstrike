#include <assert.h>
#include "dirty.h"

static int width,height;
static int drawto = 0;
static int all_dirty[2] = {1,1};
/*
  The mask at dirty_mask[drawto] contains the tiles
  currently made dirty by drawing.
  dirty_mask[1 - drawto] contains the tiles that are 
  to be restored from the background.

  1) Restore the background from dirty_mask[1 - drawto].
     Keep the mask for later use. (dirty_restore())
  2) Paint the screen and store affected tiles in dirt_mask[drawto].
  3) Add the newly dirty rectangles to dirty_mask[1 - drawto]
  4) SDL_UpdateRects() the tiles in dirty_mask[1 - drawto]
  5) Zero dirty_mask[1 - drawto] and then flip drawto

*/

static unsigned long int dirty_mask[2][32];
static SDL_Rect tile_rects[512]; /* 512 is the maximum number of rects
				    in a 32x32 tile array */
static int nr_tile_rects = 0;

void dirty_setup(int xsize, int ysize) 
{
  assert(xsize < 32*DIRTY_TILE_SIZE);
  assert(ysize < 32*DIRTY_TILE_SIZE);
  width = xsize;
  height = ysize;
}

void dirty_all()
{
  all_dirty[drawto] = 1;
}

void dirty_all_bg()
{
  all_dirty[1 - drawto] = 1;
}

/* flips the drawto counter, registers the restored rects in the
   new dirty_mask[drawto] array. */
void dirty_restore(SDL_Surface *target, SDL_Surface *background, 
		   int bg_xoff, int bg_yoff)
{
  SDL_Rect r,rb;
  unsigned long int mask;
  int row;

  if (all_dirty[1 - drawto])
    {
      r.x = bg_xoff;
      r.y = bg_yoff;
      r.w = target->w;
      r.h = target->h;
      SDL_BlitSurface(background,&r,target,0);
    }
  else
    {
      for (row = 0; row < 32; row++)
	{
	  r.y = row*DIRTY_TILE_SIZE;
	  r.h = DIRTY_TILE_SIZE;
	  r.x = 0;
	  r.w = 0;
	  
	  mask = dirty_mask[1 - drawto][row];
	  
	  while (mask)
	    {
	      while (mask && ((mask & 1) == 0))
		{
		  r.x += DIRTY_TILE_SIZE;
		  mask /= 2;
		}
	      while (mask && (mask & 1))
		{
		  r.w += DIRTY_TILE_SIZE;
		  mask /= 2;
		}
	      rb.x = r.x + bg_xoff;
	      rb.y = r.y + bg_yoff;
	      rb.w = r.w;
	      rb.h = r.h;
	      SDL_BlitSurface(background,&rb, target,&r);
	      r.x += r.w;
	      r.w = 0;
	    }
	}
    }
}

void dirty_rect(SDL_Rect *r)
{
  if ((!all_dirty[drawto]) && (r->w > 0) && (r->h > 0) 
      && (r->x < width)
      && (r->y < height))
    {
      int xs,xe,ys,ye,i,j;
      xs = r->x / DIRTY_TILE_SIZE;
      ys = r->y / DIRTY_TILE_SIZE;  
      xe = (r->x + r->w - 1) / DIRTY_TILE_SIZE;
      ye = (r->y + r->h - 1) / DIRTY_TILE_SIZE;
      for (i=ys;i<=ye;i++)
	for (j = xs; j <= xe; j++)
	  dirty_mask[drawto][i] |= (unsigned long)1 << j;
    }
}

void dirty_rect_bg(SDL_Rect *r)
{
  int then = 1 - drawto;
  if ((!all_dirty[then]) && (r->w > 0) && (r->h > 0) 
      && (r->x < width)
      && (r->y < height))
    {
      int xs,xe,ys,ye,i,j;
      xs = r->x / DIRTY_TILE_SIZE;
      ys = r->y / DIRTY_TILE_SIZE;  
      xe = (r->x + r->w - 1) / DIRTY_TILE_SIZE;
      ye = (r->y + r->h - 1) / DIRTY_TILE_SIZE;
      for (i=ys;i<=ye;i++)
	for (j = xs; j <= xe; j++)
	  dirty_mask[then][i] |= (unsigned long)1 << j;
    }
}

static INLINE void update_row(SDL_Surface *target, int row)
{
  SDL_Rect r;
  unsigned long int mask = dirty_mask[1 - drawto][row];

  r.y = row*DIRTY_TILE_SIZE;
  if ((row + 1)*DIRTY_TILE_SIZE <= target->h)
    {
      r.h = DIRTY_TILE_SIZE;
    }
  else
    {
      r.h = DIRTY_TILE_SIZE - ((row + 1)*DIRTY_TILE_SIZE - target->h);
    }
  r.x = 0;
  r.w = 0;
  
  while (mask)
    {
      while (mask && ((mask & 1) == 0))
	{
	  r.x += DIRTY_TILE_SIZE;
	  mask /= 2;
	}
      while (mask && (mask & 1))
	{
	  r.w += DIRTY_TILE_SIZE;
	  mask /= 2;
	}
      tile_rects[nr_tile_rects++] = r;
      r.x += r.w;
      r.w = 0;
    }
  if (nr_tile_rects)
    {
      if (tile_rects[nr_tile_rects - 1].x + 
	  tile_rects[nr_tile_rects - 1].w > target->w)	  
      {
	tile_rects[nr_tile_rects - 1].w -= 
	  tile_rects[nr_tile_rects - 1].x + 
	  tile_rects[nr_tile_rects - 1].w - target->w;
      }
    }
}

/* Makes sure the dirty rects are visible and schedule them for
 * background update next frame.
 */
void dirty_update(SDL_Surface *target)
{
  int i,row;
  unsigned long int mask;
  all_dirty[1 - drawto] |= all_dirty[drawto];
  if (all_dirty[1 - drawto])
    {
      SDL_UpdateRect(target,0,0,0,0);
      all_dirty[1 - drawto] = 0;
    }
  else
    {
      for (row = 0; row < 32; row++)
	dirty_mask[1 - drawto][row] |= dirty_mask[drawto][row];
      nr_tile_rects = 0;
      for (i=0;i<32;i++)
	update_row(target,i);
      SDL_UpdateRects(target,nr_tile_rects,tile_rects);
    }
  for (row = 0; row < 32; row++)
    dirty_mask[1 - drawto][row] = 0;
  drawto = 1 - drawto;
}

