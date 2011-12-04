#include "teams.h"
#include "players.h"
#include "prototype.h"

team_t teams[MAXTEAMS];

void teams_init(int nbTeams, int nbPlayers)
{
	int i;
	for (i = 0; i < nbTeams; i++)
	{
		teams[i].id = i;
		teams[i].points = 0;

		if (i < nbPlayers % nbTeams)
		{
			teams[i].nbPlayers = (int) (nbPlayers / nbTeams) + 1;
		}
		else
		{
			teams[i].nbPlayers = (int) (nbPlayers / nbTeams);
		}

		//teams[i].members = (player_t*) malloc(nbPlayers);
	}
}
/*
void addPlayers(int nbTeams)
{
	int i, j, p = 0;
	for (i = 0; i < nbTeams; i++)
	{
		for (j = 0; j < teams[i].nbPlayers; j++)
		{
			teams[i].members[j] = players[p];
			p++;
		}
	}
}
*/
