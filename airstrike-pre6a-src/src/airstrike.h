#ifndef BIP_H
#define BIP_H

#include "control.h"

extern  sprite_group_t *mech_group;
extern  sprite_group_t *bullet_group;
extern  sprite_group_t *effects_group;
extern  sprite_group_t *foreground_group;
extern  sprite_group_t *ui_group;

extern sprite_t *p1_sprite;
extern sprite_t *p2_sprite;
extern int p1_lives;
extern int p2_lives;
extern char *p1_name;
extern char *p2_name;
extern controller_t *p1_controller;
extern controller_t *p2_controller;


#endif
