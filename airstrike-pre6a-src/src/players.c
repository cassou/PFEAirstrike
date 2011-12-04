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
<<<<<<< HEAD
		players[i].teamId = 1;
=======
		players[i].team=0;
>>>>>>> 91b954ac3b084423fbca5db9ef5ed0eb968909ec
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

