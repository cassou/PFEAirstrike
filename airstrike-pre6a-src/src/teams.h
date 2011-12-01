#ifndef TEAM_H_
#define TEAM_H_

#include "players.h"

#define MAXTEAMS 10

typedef struct
{
	int id;
	int points;
	int nbPlayers;
	player_t **members;
} team_t;

extern team_t teams[MAXTEAMS];

void teams_init(int nbTeams, int nbPlayers);

#endif /* TEAM_H_ */
