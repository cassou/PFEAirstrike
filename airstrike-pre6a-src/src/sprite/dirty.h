
/*
  1) Blit to screen. Call dirty_rect() for each place
  2) dirty_update() to refresh the display.
  3) dirty_restore() to restore background. Mark these as dirty.

  Use dirty_*_bg() if the background changes.
 */

#ifndef DIRTY_H
#define DIRTY_H
#include <SDL/SDL.h>

#define DIRTY_TILE_SIZE 64

void dirty_setup(int xsize, int ysize);
void dirty_rect(SDL_Rect *r);
void dirty_all();
void dirty_rect_bg(SDL_Rect *r);
void dirty_all_bg();
void dirty_update(SDL_Surface *target);
void dirty_restore(SDL_Surface *target, SDL_Surface *background, 
		   int bg_xoff, int bg_yoff);

#endif
