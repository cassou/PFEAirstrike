#ifndef SIGNALS_H
#define SIGNALS_H

/* This struct may be used as data when a signal
   asks a question from the sprite */
struct signal_reply
{
  int reply;
  void *data;
};

/* Defines some signals to use with sprite_signal(). In general
 * -SIGNAL_TYPE means the opposite of SIGNAL_TYPE. It is never
 * guaranteed that a sprite will catch a specific signal, this must be
 * implements as needed.
 */
enum sprite_signals{
  SIGNAL_IGNORE = 0, /* Should never have any effect */
  SIGNAL_DEBUG, /* Target should print info about itself to stderr */
  SIGNAL_ALARM, /* A previously set alarm was activated. */
  SIGNAL_DAMAGE,  /* data is a pointer to an int with the amount */
  SIGNAL_POSTARGET,   /* data is a pointer to position (int[2]) */
  SIGNAL_SPRITETARGET,/* data points to a sprite */
  SIGNAL_RESTORE, /* The sprite should return to its initial state */
  SIGNAL_BGSTUCK, /* This sprite is somehow completely inside the
		     background. It should be restored or killed. */ 
  SIGNAL_KILL, /* The sprite should kill itself */

  SIGNAL_CANCONTROL, /* set the int in data to 1 if the sprite
			accepts control signals */
  /* The following signals are used by the controllers. They all have
     data=0. -SIGNAL is sent when the key is released */
  SIGNAL_FIRE,
  SIGNAL_UP,
  SIGNAL_DOWN,
  SIGNAL_LEFT,
  SIGNAL_RIGHT,
  SIGNAL_ACCELERATE,
  SIGNAL_BREAK,
  SIGNAL_STOP,
  SIGNAL_NEXT,
  SIGNAL_PREV,
  SIGNAL_ACTIVATE,
  SIGNAL_DEACTIVATE,
  SIGNAL_NUM0,
  SIGNAL_NUM1,
  SIGNAL_NUM2,
  SIGNAL_NUM3,
  SIGNAL_NUM4,
  SIGNAL_NUM5,
  SIGNAL_NUM6,
  SIGNAL_NUM7,
  SIGNAL_NUM8,
  SIGNAL_NUM9,
  /* Question signals. data is a struct signal_reply with .data as the
     asking sprite. It is never guaranteed that the target replies.
     These signals should never be used through an alarm, since data
     may be invalid when the signal is delivered. */
  SIGNAL_ISHARMLESS, /* Are you harmless to .data ? */
  SIGNAL_ISFRIEND, /* Are you a friend of .data ? */
  /* When a sprite-specific sound ends send this signal */
  SIGNAL_ENDOFSOUND,
  /* Sent when a sprite is moved because is has moved out of the
     level area */
  SIGNAL_LEVELWARP,
  /* Write a status string of this sprite into buffer at data.
   The buffer has a length of 256, including the terminating 0. */
  SIGNAL_STATSTRING,
};

#endif
