/*
 *                         bitmask.c 1.2 
 *                         -------------
 *  Simple and efficient bitmask collision detection routines
 *  Copyright (C) 2002-2003 Ulf Ekstrom except for the bitcount function.
 *
 *  A bitmask is a simple array of bits, which can be used for 
 *  2d collision detection. 
 *
 *  To use just create a mask for each object using bitmask_create()
 *  and bitmask_setbit(). The masks are cleared when created, so you
 *  just need to set the 'occupied' bits. You can then use the
 *  bitmask_overlap* functions to check for overlap.
 *
 *  The current implementation uses 32 bit wide stripes to hold  
 *  the masks, but should work just as well with 64 bit sizes.
 *  (Note that the current bitcount function is 32 bit only!)
 *
 *  The overlap tests uses the following offsets (which may be negative):
 *
 *   +----+----------..
 *   |A   | yoffset   
 *   |  +-+----------..
 *   +--|B        
 *   |xoffset      
 *   |  |
 *   :  :  
 *
 *  For optimal collision detection performance combine these functions
 *  with some kind of pre-sorting to avoid comparing objects far from 
 *  each other.
 *
 *  BUGS: No known bugs, even though they may certainly be in here 
 *  somewhere. The library need more testing on big endian machines.
 *
 *  Possible performance improvements could be to implement 
 *  wider stripes if the masks used are wider than 64 bits on the average.
 *  The bits can then be stored interlaced in two 32 bit words, and
 *  depending on the xoffset we only need to check odd-odd and even-even
 *  or odd-even/even-odd bits, saving 1/3 of the checks.
 *   
 *
 *  Performance of the various functions goes something like: 
 *  (relative timings, smaller is better)
 * 
 *  bitmask_overlap()        1.0
 *  bitmask_overlap_pos()    1.3
 *  bitmask_overlap_area()   1.6
 *
 *  For maximum performance on my machine I use gcc with
 *  -O2 -fomit-frame-pointer -funroll-loops 
 *
 *  If you can make these functions faster or have found any bugs please
 *  email Ulf Ekstrom, ulfek@ifm.liu.se. If you have made some
 *  optimization please benchmark its gain before submitting it! 
 *
 *  How to calculate an angle of collision
 *  --------------------------------------
 *  An approximate collision normal can be found by doing
 *
 *  int dx = bitmask_overlap_area(a,b,xoff+1,yoff) - 
 *           bitmask_overlap_area(a,b,xoff-1,yoff);
 *  int dy = bitmask_overlap_area(a,b,xoff,yoff+1) - 
 *           bitmask_overlap_area(a,b,xoff,yoff-1);
 *
 *  The vector (dx,dy) will be a good collision normal (it is probably
 *  hard to do better just from the information in the masks).
 *
 *  Changelog
 *  ---------
 *
 *  1.2 Removed all uses of integer division from bitmask_overlap_pos()
 *      Made small portability fix for other than 32 bit machines.
 *      The library has now seen some real world use, and has no known
 *      bugs.
 *
 *  1.1 Fixed bug where width and height of a mask was mixed up.
 *  1.0 Initial version
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifndef BITMASK_H
#define BITMASK_H
#include <limits.h>

/* Define INLINE for different compilers. */
#ifndef INLINE
# ifdef __GNUC__
#  define INLINE __inline__
# else
#  ifdef _MSC_VER
#   define INLINE __inline
#  else
#   define INLINE
#  endif
# endif
#endif

#define BITW unsigned long int
#define BITW_LEN (sizeof(BITW)*CHAR_BIT)
#define BITW_MASK (BITW_LEN - 1)
#define BITN(n) ((BITW)1 << (n))

typedef struct bitmask
{
  int w,h;
  BITW *bits;
} bitmask;

/* Creates a bitmask of width w and height h.
 * The mask is automatically cleared when created.
 */
bitmask *bitmask_create(int w, int h);
void bitmask_free(bitmask *m);

void bitmask_clear(bitmask *m);
void bitmask_fill(bitmask *m);

/* Returns nonzero if the bit at (x,y) is set. 
 * Coordinates start at (0,0)
 */
static INLINE int bitmask_getbit(const bitmask *m,int x,int y) 
{ 
  return m->bits[x/BITW_LEN*m->h + y] & BITN(x & BITW_MASK); 
}


/* Sets the bit at (x,y) */
static INLINE void bitmask_setbit(bitmask *m,int x,int y)
{ 
  m->bits[x/BITW_LEN*m->h + y] |= BITN(x & BITW_MASK); 
}


/* Clears the bit at (x,y) */
static INLINE void bitmask_clearbit(bitmask *m,int x,int y)
{ 
  m->bits[x/BITW_LEN*m->h + y] &= ~BITN(x & BITW_MASK); 
}


/* Returns nonzero if the masks overlap with the given offset. */
int bitmask_overlap(const bitmask *a,const bitmask *b,int xoffset, int yoffset);

/* Like bitmask_overlap(), but will also give a point of intersection.
 * x and y are given in the coordinates of mask a, and are untouched
 * if there is no overlap.
 */
int bitmask_overlap_pos(const bitmask *a,const bitmask *b,int xoffset, int yoffset, int *x, int *y);

/* Returns the number of overlapping 'pixels' */
int bitmask_overlap_area(const bitmask *a,const bitmask *b,int xoffset, int yoffset);

/* Draws mask b onto mask a (bitwise OR) 
 * Can be used to compose large (game background?) mask from 
 * several submasks, which may speed up the testing. 
 */
void bitmask_draw(bitmask *a,bitmask *b,int xoffset, int yoffset);

#endif
