#ifndef PLAYERS_H
#define PLAYERS_H

#include "sprite.h"
#include "control.h"
#include "teams.h"

#define MAXPLAYERS 128


typedef struct player
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
	struct team_t *team;
} player_t;

extern player_t players[MAXPLAYERS];

void player_init();
void player_setai(int playerId);
void player_sethuman(int playerId);

#endif
