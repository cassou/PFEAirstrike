#ifndef CONTROL_H
#define CONTROL_H

#include "sprite.h"

#define MAX_CONTROLLER_KEYS 8

typedef struct controller
{
  sprite_t *target;
  void (*update)(struct controller *controller);
  void (*destroy)(struct controller *controller);
} controller_t;

/* This array defines the /names/ of the controller signals*/
extern struct ckn
{
  int signal;
  char *name;
} control_key_names[];

controller_t *keyboard_controller_create(int player);
controller_t *ai_controller_create();
void ai_controller_set_enemy(controller_t *c,sprite_t *target);

#endif
