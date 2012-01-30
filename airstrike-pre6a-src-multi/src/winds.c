/*
 * Wind zones with different (random) wind changes in each zone,
 * for Airstrike.
 * 
 * TODO (search for TODO in code):
 * - Take into account object size when calculating wind effect.
 *   Object size should be it's frame size (i.e. empty lines & cols
 *   should be removed from sprite size). This not done here.
 * - Should the max_wind_change be relative to (max - min)?
 * 
 * Notes:
 * - winds_setup() has to be called before
 *   any other wind functions are called.
 * - The wind in each zone is a linear average of winds at the zone
 *   edges. The last zone is averaged with the first one, so that
 *   wind doesn't change suddenly when objects wrap around screen,
 *   see wind_get().
 * - At the moment wind zones are split evenly over the screen and
 *   there's a single max strenght, changing this would require only
 *   changing winds_setup().  Configuring induvidual zone positions and
 *   min/max strengts might be too much hazzle for the user though...
 * 
 * 2002 (C) Eero Tamminen
 */
#include <stdlib.h>
#include <assert.h>
#include "config.h"
#include "level.h"
#include "winds.h"

typedef struct {
	int count;		/* nro. of wind zones */
	int *off;		/* zone edge offsets, have to be ordered */
	float *val;		/* wind value at given position */
	float *min;		/* min. allowed wind at given position */
	float *max;		/* max. allowed wind at given position */
} windzones_t;

static windzones_t h_wind, v_wind;
static float max_wind_change;	/* max. wind change / game time frame */


/* wind_change
 * val -- original value
 * chg -- maximum change
 * min -- minimum value
 * max -- maximum value
 * returns value with random change limited to given min/max range
 */
static float wind_change(float val, float min, float max, float chg)
{
	val += ((rand() * chg * 2.0 / RAND_MAX) - chg);
	if (val > max) {
		val = max;
	}
	if (val < min) {
		val = min;
	}
	return val;
}


/* change horizontal and vertical winds by given random max value
 * and check that result is within given bounds
 */
void winds_change(void)
{
	int i;
	float *val, *min, *max;
	
	val = h_wind.val;
	min = h_wind.min;
	max = h_wind.max;
	for (i = 0; i < h_wind.count; i++) {
		*val = wind_change(*val, *min, *max, max_wind_change);
		val++;
		min++;
		max++;
	}
	val = v_wind.val;
	min = v_wind.min;
	max = v_wind.max;
	for (i = 0; i < v_wind.count; i++) {
		*val = wind_change(*val, *min, *max, max_wind_change);
		val++;
		min++;
		max++;
	}
}


/* wind_get()
 * x -- object position on screen
 * screen -- screen size
 * zones -- number of wind zones
 * zoff -- zone screen offsets
 * zval -- wind values for zones
 * 
 * returns wind affecting object with given size at given position
 */
static float wind_get(int x, int screen, int zones, int *zoff, float *zval)
{
	int i, a, b;

	/* assert that there are wind zones and that object is within them */
	assert(zones > 0);
	if (x < 0) {
		x = 0;
	}
	if (x >= screen) {
		x = screen-1;
	}
	
	/* there are so few wind zones that linear search is fastest */
	for(i = zones-1; i > 0; i--) {
		if (x > zoff[i]) {
			break;
		}
	}
	if (x == zoff[i]) {
		/* exactly on the zone edge */
		return zval[i];
	}

	/* linear approximation of wind */
	if (i >= zones-1) {
		//fprintf(stderr, "%d within [%d,%d](%d)\n", x, zoff[i], screen, i);

		/* wraparound */
		a = x - zoff[i];
		b = screen - x;
		return (b * zval[i] + a * zval[0]) / (a + b);
	} else {
		//fprintf(stderr, "%d within [%d,%d](%d)\n", x, zoff[i], zoff[i+1], i);

		a = x - zoff[i];
		b = zoff[i+1] - x;
		return (b * zval[i] + a * zval[i+1]) / (a + b);
	}
}


/* get_object_wind()
 * x -- object x position within screen
 * y -- object y position within screen
 * wind -- wind x,y value array
 * 
 * sets wind according to given object position and size
 */
void get_object_wind(int x, int y, float *wind)
{
	wind[0] = wind_get(x, level.xmax, h_wind.count, h_wind.off, h_wind.val);
	wind[1] = wind_get(y, level.ymax, v_wind.count, v_wind.off, v_wind.val);
	
	/* TODO: scale wind according to object size */
}

	
/* free wind zone arrays */
void winds_exit(void)
{
	if (h_wind.count) {
		free(h_wind.off);
		h_wind.count = 0;
	}
	if (v_wind.count) {
		free(v_wind.off);
		v_wind.count = 0;
	}
}


/* returns random value between min and max */
static inline float wind_random(float min, float max)
{
	return (rand() * (max - min) / RAND_MAX) + min;
}


/* winds_setup()
 * 
 * read configuration values, allocate wind zone arrays and
 * initialize them with random values
 * 
 * returns true on success, zero otherwise
 */
int winds_setup(void)
{
	int alloc_size = sizeof(int) + sizeof(float) * 3;
	int i, size, off, h_zones, v_zones;
	float max_change, max_wind;

	h_zones = cfgnum("level.horizontal_winds", 3);
	v_zones = cfgnum("level.vertical_winds", 3);
	max_change = cfgnum("level.max_wind_change", 0);
	max_wind = cfgnum("level.max_wind", 0);

	assert(h_zones > 0 && v_zones > 0);
	
	/* horizontal wind zones */
	h_wind.off = malloc(alloc_size * h_zones);
	if (!h_wind.off) {
		return 0;
	}
	h_wind.max = (float*)(h_wind.off + h_zones);
	h_wind.min = (float*)(h_wind.max + h_zones);
	h_wind.val = (float*)(h_wind.min + h_zones);
	
	off = 0;
	size = level.xmax / h_zones;
	for (i = 0; i < h_zones; i++) {
		h_wind.val[i] = wind_random(-max_wind, max_wind);
		h_wind.min[i] = -max_wind;
		h_wind.max[i] = max_wind;
		h_wind.off[i] = off;
		off += size;
	}
	h_wind.count = h_zones;

	/* vertical wind zones */
	v_wind.off = malloc(alloc_size * v_zones);
	if (!v_wind.off) {
		return 0;
	}
	v_wind.max = (float*)(v_wind.off + v_zones);
	v_wind.min = (float*)(v_wind.max + v_zones);
	v_wind.val = (float*)(v_wind.min + v_zones);
	
	off = 0;
	size = level.ymax / v_zones;
	for (i = 0; i < v_zones; i++) {
		v_wind.val[i] = wind_random(-max_wind, max_wind);
		v_wind.min[i] = -max_wind;
		v_wind.max[i] = max_wind;
		v_wind.off[i] = off;
		off += size;
	}
	v_wind.count = v_zones;
	
	max_wind_change = max_change;
	return 1;
}
