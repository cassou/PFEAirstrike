#include <assert.h>
#include "sprite_types.h"
#include "players.h"

int player_ishuman[MAXPLAYERS] = {1,0};
int player_points[MAXPLAYERS] = {0,0};
sprite_t *player_sprite[MAXPLAYERS] = {0,0};
char *player_name[MAXPLAYERS] = {"Blue Baron","Red Baron"};
controller_t *player_controller[MAXPLAYERS] = {0,0};
int player_keymap[MAXPLAYERS][MAX_CONTROLLER_KEYS][2] = 
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
  };
sprite_type_t *player_sprite_type[MAXPLAYERS] = {0,0};
int player_startpos[MAXPLAYERS][2];

void player_setai(int player)
{
  assert((player >= 0) && (player < MAXPLAYERS));

  if (player_controller[player])
    player_controller[player]->destroy(player_controller[player]);

  player_controller[player] = ai_controller_create();
  sprite_aquire(player_sprite[player],&(player_controller[player]->target));

  player_ishuman[player] = 0;
}

void player_sethuman(int player)
{
  assert((player >= 0) && (player < MAXPLAYERS));

  if (player_controller[player])
    player_controller[player]->destroy(player_controller[player]);

  player_controller[player] = keyboard_controller_create(player);
  sprite_aquire(player_sprite[player],&(player_controller[player]->target));

  player_ishuman[player] = 1;
}

