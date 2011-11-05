/*
 * Wind zones with different (random) wind changes in each zone.
 *
 * Usage:
 * - call winds_setup() to initialize wind zones
 * - call get_object_wind() for each object affected by the wind
 * - call winds_change() once for each game time frame
 *(- call winds_exit() to free wind zones)
 */
#ifndef WINDS_H
#define WINDS_H


/* winds_setup()
 * 
 * read configuration values, allocate wind zone arrays and
 * initialize them with random values
 * 
 * returns true on success, zero otherwise
 */
extern int winds_setup(void);
extern void winds_exit(void);


/* get_object_wind()
 * x -- object x position within screen
 * y -- object y position within screen
 * wind -- wind x,y value array
 * 
 * sets wind according to given object position and size
 */
extern void get_object_wind(int x, int y, float *wind);

extern void winds_change(void);


#endif	/* WINDS_H */
