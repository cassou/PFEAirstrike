#include <assert.h>
#include "sprite_types.h"
#include "players.h"
#include "prototype.h"
#include "control.h"

player_t players[MAXPLAYERS];

void player_init(){
	int i;
	for (i=0;i<playerCount;i++)
	{
		strcpy(players[i].name,"Joueur x");
		players[i].ishuman=1;
		players[i].points=0;
		players[i].sprite=0;
		players[i].controller=0;
		players[i].sprite_type=0;
		players[i].id=i;
		players[i].team=0;
		players[i].damage=0;
	//	players[i].timeBeforeRespawn=TIMEFIRSTSPAWN;
		players[i].spawnTimer=NULL;
		players[i].isConnected=0;
		sprintf(players[i].name,'\0');
		//sprintf(players[i].name,"nyan-cat\0",i);
		//printf("OOOOOOOOOOOOOOOOOOOO%s\n",players[i].name);
		players[i].schrodinger=1;

	}
}

void player_setai(int playerId)
{
	assert((playerId >= 0) && (playerId < MAXPLAYERS));

	player_t * player= &players[playerId];

	if (player->controller)
		player->controller->destroy(player->controller);

	player->controller = ai_controller_create();
	sprite_aquire(player->sprite,&(player->controller->target));

	player->ishuman = 0;
}

void player_sethuman(int playerId)
{
	assert((playerId >= 0) && (playerId < MAXPLAYERS));

	player_t * player= &players[playerId];

	if (player->controller)
		player->controller->destroy(player->controller);

	player->controller = keyboard_controller_create(playerId);
	sprite_aquire(player->sprite,&(player->controller->target));

	player->ishuman = 1;

}

