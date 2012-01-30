#include <assert.h>
#include <SDL/SDL_image.h>
#include "console.h"
#include "sprite.h"
#include "text.h"

static char lines[CONSOLE_LINES][CONSOLE_COLUMNS+1];
static int firstline;
static int atline,atcol; /*atline is counted _from fistline_ */
static int xpos,ypos;
static SDL_Surface *bg = 0;

int console_setup()
{
  firstline = 0;
  return 0;
}

void console_set_pos(int x, int y)
{
  xpos = x;
  ypos = y;
}

void console_load_bg(char *filename)
{
  SDL_Surface *tmp;
  assert(tmp  = IMG_Load(filename));
  bg = SDL_DisplayFormatAlpha(tmp);
  SDL_FreeSurface(tmp);
}

void console_draw()
{
  int row,y = ypos;
  SDL_Rect r;
  if (bg)
    {
      r.x = xpos - 4;
      r.y = ypos - 4;
      r.w = bg->w;
      r.h = bg->h;
      SDL_BlitSurface(bg,0,sprite_global.display,&r);
      sprite_dirty(&r);
    }

  r.x = xpos;
  r.y = ypos;
  row = firstline;
  do
    {
      text_render(sprite_global.display,&r,big_font,
		  xpos,y,ALIGN_LEFT,ALIGN_TOP,
		  lines[row]);
      y += r.h;
      row = (row + 1) % CONSOLE_LINES;
    }
  while (row != firstline);
}

void console_update()
{

}

void console_write(const char *s)
{
  while (*s)
    {
      assert(atcol <= CONSOLE_COLUMNS);
      if ((atcol >= CONSOLE_COLUMNS) || (*s == '\n'))
	{
	  lines[(firstline + atline) % CONSOLE_LINES][atcol] = 0;
	  atline++;
	  atcol = 0;
	  lines[(firstline + atline) % CONSOLE_LINES][atcol] = 0;
	  if (atline >= CONSOLE_LINES)
	    {
	      atline = CONSOLE_LINES - 1;
	      firstline = (firstline + 1) % CONSOLE_LINES;
	    }
	}
      else
	{
	  lines[(firstline + atline) % CONSOLE_LINES][atcol++] = *s;
	}
      s++;
    }
  lines[(firstline + atline) % CONSOLE_LINES][atcol] = 0;
}

void console_clear()
{
  int i;
  firstline = 0;
  atline = 0;
  atcol = 0;
  for (i=0;i<CONSOLE_LINES;i++)
    {
      lines[i][0] = 0;
    }
}

