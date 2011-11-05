/* 
 * Sprite engine Copyright (C) 2002 Ulf Ekstr�m
 * This code is released under the GPL
 */

#ifndef SPRITE_H
#define SPRITE_H

#include <SDL/SDL.h>

extern struct sprite_global
{
  SDL_Surface *display;
  int xoff, yoff; /* The point shown in the top-left corner */
  
  SDL_Surface *bg_image;
  struct bitmask *bg_mask;
  
  unsigned int dt; /* This controls the game timestep */
  unsigned int display_dt; /* This one sets the target framerate */
  unsigned int frame_start; /* start of the frame in real time */
  unsigned int game_clock; /* the current game clock */

  /* Set options before doing stuff! */
  int opt_alpha; /* 0: no alpha, 1: per surface, 2: per pixel */
} sprite_global;

struct sprite;

#define MAGICPINK 255,0,255

typedef struct animation
{
  /*the coordinates of the 'origin' of this frame (center by default)*/
  int xoff, yoff;
  unsigned int delay;
  int nr_pframes;
  SDL_Surface **images;
  struct bitmask **masks;
  struct animation *next_frame;
  struct animation *base_frame;
  /* The trigger is called when this frame is _entered_, if nonzero */
  void (*trigger)(struct sprite *s); 
} animation_t;

struct sprite_world;

typedef struct sprite_type 
{
  char *name;
  int (*setup)(void);   /* Returns 0 on success */
  struct sprite *(*create_sprite)(void);
  /* Called when refcount reaches zero */
  void (*free_sprite)(struct sprite *s); 
  /* this_sprite is guaranteed to have this type.
     x and y gives a point of collision in global coordinates. */
  void (*collide)(struct sprite *this_sprite, 
		  struct sprite *other_sprite, int x, int y);
  void (*bg_collide)(struct sprite *this_sprite, int x, int y);
  void (*update)(struct sprite *s);
  void (*signal)(struct sprite *s, int signal, void *data);
  int _setup_ok; /* 0: not yet setup, 1: ok. We cannot continue if it
		    fails, so no need to handle that case */
} sprite_type_t;

typedef struct sprite
{
  /* 'public' variables */
  unsigned int state; /* User defined. */
  int x,y; /* pixel coordinates */
  animation_t *animation;
  int anim_p; /* Animation parameter (angle etc) */
  sprite_type_t *type;
  float pos[2];
  float vel[2];

  /* 'private' variables */
  int _t_off;
  int refcount;
} sprite_t;

typedef struct sprite_group 
{
  sprite_t **sprites; 
  int max_sprites; /* the size of the sprites array */
  int nr_sprites; /* number of sprites in this group */
} sprite_group_t;

typedef unsigned int sprite_timer_t;

enum sprite_detector_type 
  {
    SPRITE_DETECTOR_SPHERE8 = 0,
    SPRITE_DETECTOR_SPHERE16,
    SPRITE_DETECTOR_SPHERE32,
    SPRITE_DETECTOR_SPHERE64,
    /* ... */
    SPRITE_DETECTOR_NARROW_CONE8,
    SPRITE_DETECTOR_NARROW_CONE16,
    SPRITE_DETECTOR_NARROW_CONE32,
    /* ... */
    SPRITE_DETECTOR_WIDE_CONE8,
    SPRITE_DETECTOR_WIDE_CONE16,
    SPRITE_DETECTOR_WIDE_CONE32,
    /* ... */
    SPRITE_DETECTOR_RAY16,
    SPRITE_DETECTOR_RAY32,
  };

/* animation functions */

struct animation *animation_load(const char *imagefile, int nr_pframes, 
				 int nr_tframes, unsigned int ms_delay);
struct animation *animation_last_frame(struct animation *anim);
/* Returns frame nr n, starting from 0 */
struct animation *animation_frame_n(struct animation *anim, int frame_nr);
void animation_make_loop(animation_t *anim);
void animation_set_alpha(struct animation *a, unsigned char alpha);
void animation_set_center(struct animation *a, int x, int y);
void animation_draw(struct animation *a, int x, int y, int parameter);

/* sprite functions */

sprite_t *sprite_create(sprite_type_t *type);

int sprite_alive(sprite_t *s); /* Do not use outside this library */
void sprite_kill(sprite_t *s); /* Marks s as dead */

int sprite_aquire(sprite_t *src, sprite_t **dest);
/* If the sprite is dead then release it and set *ref to 0, return 0.
   Else if *ref != 0 return 1 */
int sprite_isvalid(sprite_t **ref);
void sprite_release(sprite_t **ref);

void sprite_signal(sprite_t *s, int signal, void *data);
/* Sends the signal in the first frame after delay ms */
void sprite_alarm(unsigned int delay, sprite_t *target, int signal, void *data);

void sprite_set_animation(sprite_t *s, animation_t *anim);
void sprite_set_pos(sprite_t *s, int x, int y);

/* Though these functions gives floats the internal values are kept in
   fixed point format. This assures that the move function is linear,
   i.e. that move(t1) followed by move(t2) gives the same result as
   move(t1+t2). Precision can only be lost in the float->fixpoint
   conversion in sprite_set_vel().*/
void sprite_set_vel(sprite_t *s, const float v[]);
void sprite_get_vel(sprite_t *s, float v[]);

/* Number of overlapping pixels */
int sprite_get_overlap(sprite_t *s1, sprite_t *s2);
int sprite_get_bg_overlap(sprite_t *s);

/* Points from s2 towards s1. If one of the sprites is completely inside
   the other then the normal will be zero. The same is true if they don't
   overlap. This is approx 4 times as expensive as the overlap count */
void sprite_get_collision_normal(sprite_t *s1, sprite_t *s2, float n[]);
void sprite_get_bg_collision_normal(sprite_t *s1, float n[]);

/* Moves s so that it (approximately) does not intersect the
  background. The normal is stored in n, and the original number of
  overlapping pixels is returned.  If the gradient is 0 then -nr
  pixels is returned.  Maybe make this a bit more conservative in the
  distance moved. */
int sprite_remove_from_bg(sprite_t *s, float n[]);

/* sprite group functions */

sprite_group_t *sprite_group_create(void);
void sprite_group_free(sprite_group_t *group);
/* Inserts sprite at the end, i.e. it'll be drawn on top */
void sprite_group_insert(sprite_group_t *group, sprite_t *s);
/* Removes _one_ instance of s from group. Preserves order */
void sprite_group_remove(sprite_group_t *group, sprite_t *s);
/* Removes all dead sprites from this group */
void sprite_group_cleanup(sprite_group_t *group);

/* 'foreach'-type functions: */ 
void sprite_group_move(sprite_group_t *group, unsigned int ms);
void sprite_group_pos_update(sprite_group_t *group);
void sprite_group_animate(sprite_group_t *group, unsigned int ms);
void sprite_group_signal(sprite_group_t *group, int signal, void *data);
void sprite_group_update(sprite_group_t *group);
/* if f == 0 then the types own functions are used to handle
   the collisions */
void sprite_group_coll(sprite_group_t *group,
		       void (*f)(struct sprite *this_sprite, 
				 struct sprite *other_sprite, 
				 int x, int y));
void sprite_group_coll2(sprite_group_t *group1,
			sprite_group_t *group2,
			void (*f)(struct sprite *this_sprite, 
				  struct sprite *other_sprite, 
				  int x, int y));
void sprite_group_bg_coll(sprite_group_t *group,
			  void (*f)(struct sprite *sprite,
				    int x, int y));
void sprite_group_point_coll(sprite_group_t *group,
			     int point_x,int point_y,
			     void (*f)(struct sprite *sprite, 
				       int x, int y));

void sprite_group_draw(sprite_group_t *group);

/* future functions below */

/* Place pointer to the sprites for which pred returns true in the
   target array. Returns the number of found sprites. Not yet
   needed/implemented. */
int sprite_group_select(sprite_group_t *group,
			sprite_t target[], int max_sprites, 
			int (*pred)(sprite_t *s, void *data), void *data);

/* Call trigger for each sprite intersecting the detector.  Usable to
   let the ai find close sprites etc. */
int sprite_group_detect(sprite_group_t *group,
			enum sprite_detector_type type,
			int x, int y, int angle,
			void (*trigger)(sprite_t *s, void *data),
			void *data);

int sprite_bg_detect(enum sprite_detector_type type,
		     int x, int y, int angle,
		     void (*trigger)(void *data),
		     void *data);

/* global sprite functions */

void sprite_setup(void);
void sprite_start_frame(void); /* Call at start of frame */
unsigned int sprite_end_frame(void); /* Returns the number of ms slept */

/* Marks this rectangle as dirty. Must be called between
   sprite_start_frame() and sprite_end_frame(). Only necessary for
   graphics other than sprites and background.  r _must_ be clipped to
   the screen! */
void sprite_dirty(SDL_Rect *r); 
void sprite_dirty_all(void);
void sprite_bg_dirty(SDL_Rect *r); 
void sprite_bg_dirty_all(void);

/* resize garbles the background and the mask */
void sprite_background_resize(int xsize, int ysize);
void sprite_background_free(void);
void sprite_background_compose(sprite_t *brush, int with_mask);
void sprite_background_draw(void);
void sprite_background_load(const char *image, const char *mask);

void sprite_timer_set(unsigned int *timer, unsigned int delay);
int sprite_timer_finished(unsigned int timer);

/* Center the viewport around (x,y), as good as possible */
void sprite_viewport_center(int x, int y);
void sprite_viewport_center_on(sprite_t *s);

/* Utility function which does not really belong here..
 *
 * Converts a section of src to display format, and creates a mask if
 * mask_threshold > 0 and there is either alpha or colorkey
 * information.  
 * If src has an alpha channel and alpha_threshold == 0
 * then the dest will also have an alpha channel. Else if
 * alpha_threshold > 0 the dest will have a colorkey (magic pink),
 * where alpha > alpha_threshold will be transparent.  If the source
 * does not have an alpha channel then if alpha_threshold >= 0 it is
 * assumed to be colorkeyed with magic pink, else it is just flat.
 * surface_flags will be used when setting colorkey, alpha etc.
 * If crop is used then the target surface will be cropped as long
 * as no {alpha > 0|colorkey opaque} pixels are lost. The corresponding
 * source rectangle is stored in r. (crop is not yet implemented).
 */
int GetSubSurface(SDL_Surface *src, SDL_Rect *r,
		  SDL_Surface **dest, struct bitmask **mask,
		  int alpha_threshold, int mask_threshold,
		  Uint32 surface_flags, int crop);

#endif
