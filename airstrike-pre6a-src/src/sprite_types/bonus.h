/* 
 * Bonus handling (C) 2003 by Eero Tamminen
 * This code is released under the GPL
 */
#ifndef BONUS_H
#define BONUS_H

enum {
	/* annoyances to opponent */
	BONUS_AIRBALLOON,	/* airballoon moves toward player */
	BONUS_BIRD,		/* birds move toward player */
	BONUS_CANNON,		/* cannon shoots at player */
	BONUS_CLOUD,		/* clouds move toward player */
	BONUS_UFO,		/* ufo moves towards and fires at player */
	BONUS_ZEPPELIN,		/* zeppelin moves toward player */

	/* good stuff */
	BONUS_SCORE,		/* score bonus */
	BONUS_LIFE,		/* extra life bonus */

	BONUSES			/* number of bonuses */
};

/* If structure member is set, the corresponding game objects
 * aim to given sprite, either by moving or shooting towards it.
 * 
 * Structure members are set by bonuses when player catches them
 * and reset when the pointed player sprite dies (they have to
 * call bonus_clear_aim() when being killed).
 */
extern struct global_aim_t {
	sprite_t *airballoon;
	sprite_t *bird;
	sprite_t *cannon;
	sprite_t *cloud;
	sprite_t *ufo;
	sprite_t *zeppelin;
} global_aim;

/* clears aims for the given sprite */
void global_aim_clear(sprite_t *s);

#endif /* BONUS_H */
