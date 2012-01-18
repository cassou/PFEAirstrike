#ifndef TEAM_H_
#define TEAM_H_

#define MAXTEAMS 10
#define MAXPLAYERINTEAMS 10

typedef struct team
{
	int id;
	int points;
	int nbPlayers;
} team_t;

extern team_t teams[MAXTEAMS];

void teams_init(int nbTeams, int nbPlayers);
void update_teams_score(int nbTeams,int nbPlayers);
//void addPlayers(int nbTeams);

#endif /* TEAM_H_ */
