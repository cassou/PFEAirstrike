#include <math.h>
#include <stdlib.h>
#include "maths.h"


#ifndef M_PI
# define M_PI 3.1415926535897931
#endif


float trigtable[256][2]; /* 256 <-> 360 degrees */

void maths_setup()
{
  int i;
  for (i=0; i<256; i++)
    {
      trigtable[i][0] = cos(M_PI*i/128.0);
      trigtable[i][1] = sin(M_PI*i/128.0);
    }
}


float vdot(const float *v1, const float *v2)
{
  return v1[0]*v2[0] + v1[1]*v2[1];
}
float vcross(const float *v1, const float *v2)
{
  return v1[0]*v2[1] - v1[1]*v2[0];
}

float vabs(const float *v)
{
  return sqrt(v[0]*v[0] + v[1]*v[1]);
}
float vabs2(const float *v)
{
  return v[0]*v[0] + v[1]*v[1];
}

void vnormalize(float *v)
{
  float rl = 1/vabs(v);
  v[0] *= rl;
  v[1] *= rl;
}

void vadd(float *v, const float *d) /* v += d */
{
  v[0] += d[0];
  v[1] += d[1];
}

void vmadd(float *v, float x, const float *d) /* v += x*d */
{
  v[0] += x*d[0];
  v[1] += x*d[1];
}

void vdiff(float *v, const float *v1, const float *v2) /* v = v1 - v2 */
{
  v[0] = v1[0] - v2[0];
  v[1] = v1[1] - v2[1];
}

void vmul(float *v, float x) /* v *= x */
{
  v[0] *= x;
  v[1] *= x;
}

void vrot(float *v, int angle)
{
  float x;
  angle &= 255;
  x = v[0]*trigtable[angle][0] - v[1]*trigtable[angle][1];
  v[1] = v[0]*trigtable[angle][1] + v[1]*trigtable[angle][0];
  v[0] = x;
}

int vangle(const float *v)
{
  return (int)(128.0*atan2(v[1],v[0])/M_PI) & 255;
}

float frand(void) /* return a random float from [0..1] */
{
  return (rand()/((float)RAND_MAX));
}
