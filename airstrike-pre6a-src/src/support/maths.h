#ifndef MATHS_H
#define MATHS_H

/*
 * Defines some 2d vector utility functions.
 *
 * 256 <-> 360 degrees 
 * trigtable[i][0] is the cosine, and trig[i][1] is the sine,
 * so trigtable[i] is the unit vector in the i direction
 */
extern float trigtable[256][2]; 

static INLINE float *trig(int angle)
{
  return trigtable[angle & 255];
}

#ifndef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

static INLINE float square(float x)
{
  return x*x;
}

/* This is a little unconventional at x=0 */
static INLINE float sgn(float x)
{
  if (x > 0)
    return 1;
  else 
    return -1;
}

static INLINE float hmean(float x,float y) /*harmonic mean of x and y */
{
  return x*y/(x+y);
}

void maths_setup();
float vdot(const float *v1, const float *v2);
float vcross(const float *v1, const float *v2);
float vabs(const float *v);
float vabs2(const float *v);
void vnormalize(float *v);
void vadd(float *v, const float *d); /* v += d */
void vdiff(float *v, const float *v1, const float *v2); /* v = v1 - v2 */
void vmadd(float *v, float x, const float *d); /* v += x*d */
void vmul(float *v, float x); /* v *= x */
void vrot(float *v, int angle);
int vangle(const float *v);

static INLINE void vset(float *x,const float *y)
{
  x[0] = y[0];
  x[1] = y[1];
}

float frand(void); /* return a random float from [0..1] */

#endif
