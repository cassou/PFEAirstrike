#include <assert.h>
#include <SDL/SDL_image.h>
#include "sprite.h"
#include "bitmask.h"
#include "utils.h"

static bitmask *mask_from_image_alpha(SDL_Surface *target, int threshold)
{
  Uint32 *pixel;
  int x,y;
  Uint32 alpha;
  bitmask *m;

  assert(target->format->Amask);
  assert(target->format->BitsPerPixel == 32);
  m = bitmask_create(target->w,target->h);
  if (!m)
    return 0;
  for (x = 0;x < target->w;x ++)
    for (y = 0;y < target->h; y++)
      {
	pixel = (Uint32 *)(target->pixels) + x + y*(target->pitch)/4;
	alpha = (*pixel & (target->format->Amask)) >> target->format->Ashift;
	if (alpha > (unsigned int)threshold)
	  bitmask_setbit(m,x,y);
      }
  return m;
}

/* Ugly old function. */
static bitmask *mask_from_image(SDL_Surface *surface)
{
  bitmask *b;
  int i,j;
  SDL_Rect *r,rr;
  Uint32 colorkey;

  if (surface->format->Amask)
    return mask_from_image_alpha(surface,128);

  colorkey = SDL_MapRGB(surface->format,MAGICPINK);
  assert(surface!=0);

  rr.x = 0;
  rr.y = 0;
  rr.w = surface->w;
  rr.h = surface->h;
  r = &rr;

  b = bitmask_create(r->w,r->h);
  if (!b) return 0;
  switch (surface->format->BytesPerPixel) {
  case 1: { /* Assuming 8-bpp */
    for (j = r->y; j < r->h + r->y; j++)
      {
	for (i=r->x; i < r->w + r->x; i++)
	  {
	    if (*((Uint8 *)surface->pixels + (j)*surface->pitch + i) 
		!= colorkey)
	      bitmask_setbit(b,i-r->x,j-r->y);
	  }
      }
  }
    break;
	  
    case 2:  /* Probably 15-bpp or 16-bpp */
      for (j=r->y;j<r->h+r->y;j++)
	{
	  for (i=r->x;i<r->w+r->x;i++)
	    {
	      if (*((Uint16 *)surface->pixels + (j)*surface->pitch/2 + i)
		  != colorkey)
		bitmask_setbit(b,i-r->x,j-r->y);
	    }
	}
      break;
        case 3: { /* Slow 24-bpp mode, usually not used */
      Uint8 *bufp;
      for (j=r->y;j<r->h+r->y;j++)
	{
	  for (i=r->x;i<r->w+r->x;i++)
	    {		
	      bufp = (Uint8 *)surface->pixels + (j)*surface->pitch + (i) * 3;
	      if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
		
		if (!((bufp[0] == (colorkey & 255)) &&
		    (bufp[1] == ((colorkey >> 8) & 255)) &&
		      (bufp[2] == ((colorkey >> 16) & 255))))
		  bitmask_setbit(b,i-r->x,j-r->y);
	      } else 
		{
		  if (!((bufp[2] == (colorkey & 255)) &&
		      (bufp[1] == ((colorkey >> 8) & 255)) &&
			(bufp[0] == ((colorkey >> 16) & 255))))
		    bitmask_setbit(b,i-r->x,j-r->y);
		}
	    }
	}
	}
	  break;
  case 4:  /* Probably 32-bpp */
    for (j=r->y;j<r->h+r->y;j++)
      {
	for (i=r->x;i<r->w+r->x;i++)
	  {
	    if (*((Uint32 *)surface->pixels + (j)*surface->pitch/4 + i) 
		!= colorkey)
	      {
		bitmask_setbit(b,i-r->x,j-r->y);
	      }
	  }
      }
    break;
  }
  return b;
}

static void threshold_alpha(SDL_Surface *target, int threshold)
{
  Uint32 *pixel;
  int x,y;
  Uint32 alpha;
  
  if (!(target->format->Amask)) return;

  assert(target->format->BitsPerPixel == 32);
  for (x = 0;x < target->w;x ++)
    for (y = 0;y < target->h; y++)
      {
	pixel = (Uint32 *)(target->pixels) + x + y*(target->pitch)/4;
	alpha = (*pixel & (target->format->Amask)) >> target->format->Ashift;
	if (alpha > (unsigned int )threshold)
	  *pixel |= target->format->Amask;
	else
	  *pixel &= ~target->format->Amask;
      }
}



/* Converts a section of src to display format, and creates a mask if
 * mask_threshold > 0 and there is either alpha or colorkey
 * information.  
 *
 * If src has an alpha channel and alpha_threshold == 0 then the dest
 * will also have an alpha channel.  
 *
 * Else if alpha_threshold > 0 the dest will have a colorkey (magic
 * pink), where alpha > alpha_threshold will be transparent.  
 * 
 * If the source does not have an alpha channel then if
 * alpha_threshold >= 0 it is assumed to be colorkeyed with magic
 * pink, else it is just flat.  
 *
 * surface_flags will be used when setting colorkey, alpha etc.
 *
 * If crop is used then the target surface will be cropped as long
 * as no {alpha > 0|colorkey opaque} pixels are lost. The corresponding
 * source rectangle is stored in r. (Not yet implemented).
 */
int GetSubSurface(SDL_Surface *src, SDL_Rect *r,
		  SDL_Surface **dest, bitmask **mask,
		  int alpha_threshold, int mask_threshold,
		  Uint32 surface_flags, int crop)
{
  int hasalpha;
  SDL_Surface *tmp;
  hasalpha = src->format->Amask != 0;

  /* ugly hack to get a proper target surface */
  CRITICAL(tmp = 
	 SDL_CreateRGBSurface(SDL_SWSURFACE,
			      r->w,r->h,8,0,0,0,0));
  if (!hasalpha)
    CRITICAL(*dest = SDL_DisplayFormat(tmp));
  else
    CRITICAL(*dest = SDL_DisplayFormatAlpha(tmp));
  SDL_FreeSurface(tmp);

  if (!hasalpha)
    {
      SDL_BlitSurface(src,r,*dest,0);
      if (alpha_threshold >= 0)
	{
	  SDL_SetColorKey(*dest,SDL_SRCCOLORKEY|surface_flags,
			  SDL_MapRGB((*dest)->format,255,0,255));
	}
      if (mask_threshold > 0)
	CRITICAL(*mask = mask_from_image(*dest));
      return 0;
    }
  else if (alpha_threshold == 0) /* We want alpha in the target also */
    {
      SDL_SetAlpha(src,0,0);
      SDL_BlitSurface(src,r,*dest,0);
      SDL_SetAlpha(*dest,SDL_SRCALPHA|surface_flags,0);
      if (mask_threshold > 0)
	CRITICAL(*mask = mask_from_image_alpha(*dest,mask_threshold));
      return 0;
    }
  else /* alpha source, non-alpha target */
    {
      SDL_SetAlpha(src,0,0);
      SDL_BlitSurface(src,r,*dest,0);
      threshold_alpha(*dest,alpha_threshold);
      tmp = SDL_DisplayFormat(*dest);
      SDL_FillRect(tmp,0,SDL_MapRGB(tmp->format,255,0,255));
      SDL_SetAlpha(*dest,SDL_SRCALPHA,0);
      SDL_BlitSurface(*dest,0,tmp,0);
      SDL_FreeSurface(*dest);
      *dest = tmp;
      SDL_SetColorKey(*dest,SDL_SRCCOLORKEY|surface_flags,
		      SDL_MapRGB((*dest)->format,255,0,255));
      if (mask_threshold > 0)
	CRITICAL(*mask = mask_from_image(*dest));
      return 0;
    }
}

void sprite_background_load(const char *image, const char *mask)
{
  SDL_Surface *tmp;
  
  if (sprite_global.bg_image)
    SDL_FreeSurface(sprite_global.bg_image);

  CRITICAL(tmp = IMG_Load(image));
  sprite_global.bg_image = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);

  if (sprite_global.bg_mask)
    bitmask_free(sprite_global.bg_mask);

  CRITICAL(tmp = IMG_Load(mask));
  sprite_global.bg_mask = mask_from_image(tmp);
  SDL_FreeSurface(tmp);
}

