#ifndef PLAYERS_H
#define PLAYERS_H

#include "sprite.h"
#include "control.h"
#include "teams.h"

#define MAXPLAYERS 128

<<<<<<< HEAD

typedef struct player
=======
typedef struct
>>>>>>> 91b954ac3b084423fbca5db9ef5ed0eb968909ec
{
	int id;
	int ishuman;
	int points;
	int keymap[MAX_CONTROLLER_KEYS][2];
	char name[256];
	sprite_t *sprite;
	sprite_type_t *sprite_type;
	controller_t *controller;
	int startpos[2];
<<<<<<< HEAD

	//TODO : remove this when possible
	int teamId;

=======
	struct team_t *team;
>>>>>>> 91b954ac3b084423fbca5db9ef5ed0eb968909ec
} player_t;

extern player_t players[MAXPLAYERS];

void player_init();
void player_setai(int playerId);
void player_sethuman(int playerId);

#endif
