#include <assert.h>
#include "sprite_types.h"
#include "players.h"
#include "prototype.h"

player_t players[MAXPLAYERS];


/*int player_ishuman[MAXPLAYERS];// = {1,0};
int player_points[MAXPLAYERS];// = {0,0};
sprite_t *player_sprite[MAXPLAYERS];// = {0,0};
char *player_name[MAXPLAYERS];// = {"Blue Baron","Red Baron"};
controller_t *player_controller[MAXPLAYERS];// = {0,0};

sprite_type_t *player_sprite_type[MAXPLAYERS];// = {0,0};
int player_startpos[MAXPLAYERS][2];
int player_keymap[MAXPLAYERS][MAX_CONTROLLER_KEYS][2];
*/


int player_keymap_model[MAX_CONTROLLER_KEYS][2] =
{
		{SDLK_PERIOD,SIGNAL_FIRE},
		{SDLK_LEFT,SIGNAL_UP},
		{SDLK_RIGHT,SIGNAL_DOWN},
		{SDLK_COMMA,SIGNAL_ACCELERATE},
		{SDLK_SPACE,SIGNAL_NUM0}
};

/*int player_keymap[MAXPLAYERS][MAX_CONTROLLER_KEYS][2] =
 {
    {{SDLK_PERIOD,SIGNAL_FIRE},
     {SDLK_LEFT,SIGNAL_UP},
     {SDLK_RIGHT,SIGNAL_DOWN},
     {SDLK_COMMA,SIGNAL_ACCELERATE},
     {SDLK_SPACE,SIGNAL_NUM0}},
    {{SDLK_LSHIFT,SIGNAL_FIRE},
     {SDLK_d,SIGNAL_UP},
     {SDLK_f,SIGNAL_DOWN},
     {SDLK_LCTRL,SIGNAL_ACCELERATE},
     {SDLK_TAB,SIGNAL_NUM0}}
  };*/


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

		int j;
		for (j=0;j<MAX_CONTROLLER_KEYS;j++){
			players[i].keymap[j][0]=player_keymap_model[j][0];
			players[i].keymap[j][1]=player_keymap_model[j][1];
		}
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

