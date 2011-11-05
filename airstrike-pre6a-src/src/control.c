#include <SDL/SDL.h>
#include <malloc.h>
#include <assert.h>
#include "control.h"
#include "maths.h"
#include "mech.h"
#include "signals.h"
#include "players.h"

struct ckn control_key_names[] =
  {
    {SIGNAL_FIRE,"fire"},
    {SIGNAL_UP,"turn ccw"},
    {SIGNAL_DOWN,"turn cw"},
    {SIGNAL_ACCELERATE,"accelerate"},
    {SIGNAL_NUM0,"bomb"},
    {0,0}
  };

#define CONTROLLER_MAXKEYS 16

struct keyboard_controller
{
  controller_t base;
  int player;
  /* The keymap should have entries {SDLK_key, SIGNAL, waspressed},
     with 0 as the last keycode */
  int key_waspressed[CONTROLLER_MAXKEYS];
};

static void keyboard_update(controller_t *c)
{
  char *keys;
  int i = 0;
  int player;
  struct keyboard_controller *kc;
  if (sprite_isvalid(&(c->target)))
    {
      sprite_signal(c->target,SIGNAL_CANCONTROL,&i);
      if (!i)
	{
	  sprite_release(&(c->target));
	  return;
	}
      kc = (struct keyboard_controller *)c;
      player = kc->player;
      keys = SDL_GetKeyState(0);
      for (i=0;player_keymap[player][i][0] != 0;i++)
	{
	  if (keys[player_keymap[player][i][0]])
	    {
	      sprite_signal(c->target,player_keymap[player][i][1],0);
	      kc->key_waspressed[i] = 1;
	    }
	  else if (kc->key_waspressed[i]) /* Send the 'unpress' signal */
	    {
	      sprite_signal(c->target,-player_keymap[player][i][1],0);
	      kc->key_waspressed[i] = 0;
	    }
	}
    }
}

static void destroy_kbc(controller_t *c)
{
  sprite_release(&(c->target));
  free(c);
}

controller_t *keyboard_controller_create(int player)
{
  struct keyboard_controller *kc = calloc(1,sizeof(*kc));
  kc->base.update = keyboard_update;
  kc->base.destroy = destroy_kbc;
  kc->player = player;
  return (controller_t *)kc;
}

struct ai
{
  controller_t base;
  sprite_t *enemy;
};

static void ai_update(controller_t *c)
{
  float x,delta[2];
  int i = 0;
  struct ai *ai = (struct ai *)c;
  struct signal_reply r = {0,0};
  if (!sprite_isvalid(&(c->target)))
    {
      sprite_release(&ai->enemy);
      return;
    }
  if (sprite_isvalid(&(ai->enemy)))
    {
      sprite_signal(c->target,SIGNAL_CANCONTROL,&i);
      if (i)
	{
	  /* ask the enemy if it is still a threat */
	  sprite_signal(ai->enemy,SIGNAL_ISHARMLESS,&r);
	  if (r.reply) /* We believe this sprite will never be a threat
			  again */
	    {
	      sprite_release(&(ai->enemy));
	      return;
	    }
	  /* always accelerate */
	  sprite_signal(c->target,SIGNAL_ACCELERATE,0);
	  /* Find out which way to turn towards the enemy */
	  delta[0] = c->target->x - ai->enemy->x;
	  delta[1] = c->target->y - ai->enemy->y;
	  if (vcross(delta,
		     mech_heading((mech_sprite_t *)(c->target))) < 0)
	    sprite_signal(c->target,SIGNAL_UP,0);
	  else
	    sprite_signal(c->target,SIGNAL_DOWN,0);
	  
	  /* Only fire if we have a chance of hitting */
	  x = square(vdot(delta,
		      mech_heading((mech_sprite_t *)(c->target))));
	  if  ((vabs2(delta) < 40000) && (vabs2(delta)*0.8 < x))
	    sprite_signal(c->target,SIGNAL_FIRE,0);
	}
      else
	{
	  sprite_release(&(ai->enemy));
	  sprite_release(&(c->target));
	}
    }
  else /* enemy is dead, so we fly to the top of the screen */
    {
      delta[0] = c->target->x - 400;
      delta[1] = c->target->y - 100;
      sprite_signal(c->target,SIGNAL_ACCELERATE,0);
      if (vcross(delta,
		 mech_heading((mech_sprite_t *)(c->target))) < 0)
	sprite_signal(c->target,SIGNAL_UP,0);
      else
	sprite_signal(c->target,SIGNAL_DOWN,0);
    }
}

static void destroy_aic(controller_t *c)
{
  sprite_release(&(c->target));
  sprite_release(&(((struct ai *)c)->enemy));
  free(c);
}

controller_t *ai_controller_create()
{
  struct ai *ai = calloc(1,sizeof(*ai));
  ai->base.update = ai_update;
  ai->base.destroy = destroy_aic;
  return (controller_t *)ai;
}



void ai_controller_set_enemy(controller_t *c,sprite_t *target)
{
  sprite_release(&(((struct ai *)c)->enemy));
  sprite_aquire(target,&(((struct ai *)c)->enemy));
}
