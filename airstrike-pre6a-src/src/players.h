#ifndef PLAYERS_H
#define PLAYERS_H

#include "sprite.h"
#include "control.h"

#define MAXPLAYERS 100

typedef struct
{
	int ishuman;
	int points;
	int keymap[MAX_CONTROLLER_KEYS][2];
	char name[256];
	sprite_t *sprite;
	sprite_type_t *sprite_type;
	controller_t *controller;
	int startpos[2];

} player_t;

extern player_t players[MAXPLAYERS];

/*extern int player_ishuman[MAXPLAYERS];
extern int player_points[MAXPLAYERS];
extern sprite_t *player_sprite[MAXPLAYERS];
extern char *player_name[MAXPLAYERS];
extern controller_t *player_controller[MAXPLAYERS];*/
/* key, signal */
/*extern int player_keymap[MAXPLAYERS][MAX_CONTROLLER_KEYS][2];
extern sprite_type_t *player_sprite_type[MAXPLAYERS];
extern int player_startpos[MAXPLAYERS][2];
*/

void player_init();
void player_setai(int playerId);
void player_sethuman(int playerId);

#endif
