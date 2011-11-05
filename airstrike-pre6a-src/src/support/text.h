#ifndef TEXT_H
#define TEXT_H
#include <SDL/SDL.h>

enum text_alignment
  {
    ALIGN_CENTER,
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_TOP,
    ALIGN_BOTTOM
  };

typedef struct font
{
  int char_w, char_h, char_d;
  SDL_Surface *chars[256];
} font_t;

/* Loads an image containing the chars in the chars string in that
   order. Each char is assumed to be char_width pixels wide. The
   display must be open before calling this function. char_pitch is
   the pixel distance from the start of a char to the start of the
   next. */
font_t *text_load_font(const char *image, const char *chars,
		       int char_width, int char_pitch);

int text_setup(void);

extern font_t *big_font;
extern font_t *small_font;

/* Draw a sequence of characters on the screen
 * If boundary is nonzero then it is filled with the
 * bounding rectangle of all the printed chars.
 * The align variables control the text placement with
 * regards to x and y.
 */
void text_render(SDL_Surface *target, SDL_Rect *boundary,
		 font_t *font,
		 int x, int y, 
		 enum text_alignment xalign, 
		 enum text_alignment yalign,		 
		 char *string);

#endif
