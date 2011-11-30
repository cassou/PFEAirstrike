/*
 * team.h
 *
 *  Created on: 30 nov. 2011
 *      Author: Nicolas
 */

#ifndef TEAM_H_
#define TEAM_H_

#define MAXTEAMS 10


typedef struct
{
	int id;
	char colour[10]; // Colour of the team
	int points;
	int nbPlayers;

} team_t;

extern player_t teams[MAXTEAMS];

#endif /* TEAM_H_ */
