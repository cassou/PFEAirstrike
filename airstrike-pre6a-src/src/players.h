#ifndef PLAYERS_H
#define PLAYERS_H

#include "sprite.h"
#include "control.h"

#define MAXPLAYERS 2
  
extern int player_ishuman[MAXPLAYERS];
extern int player_points[MAXPLAYERS];
extern sprite_t *player_sprite[MAXPLAYERS];
extern char *player_name[MAXPLAYERS];
extern controller_t *player_controller[MAXPLAYERS];
/* key, signal */
extern int player_keymap[MAXPLAYERS][MAX_CONTROLLER_KEYS][2]; 
extern sprite_type_t *player_sprite_type[MAXPLAYERS];
extern int player_startpos[MAXPLAYERS][2];

void player_setai(int player);
void player_sethuman(int player);

#endif
