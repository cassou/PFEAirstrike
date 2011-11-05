#include "sound.h"

#ifdef USE_SOUND
#include <math.h>
#include "signals.h"

/* define sounds below */

sound_t sound_enginehumm = {"sound/hummm.wav",-1};
sound_t sound_enginebroken = {"sound/huvum.wav",-1};
sound_t sound_bombdrop = {"sound/twirp.wav"};
sound_t sound_gunfire = {"sound/shoot.wav"};
sound_t sound_mechbump = {"sound/splat2a.wav"}; /* when two sprites collide softly */
sound_t sound_mechsmash = {"sound/splat3a.wav"}; /* hard collision */
sound_t sound_mechgroundbump = {"sound/splat2a.wav"}; /* soft bg coll */
sound_t sound_mechgroundsmash = {"sound/splat3a.wav"}; /* hard collision*/
sound_t sound_bullethit = {"sound/klank2.wav"};
sound_t sound_bird = {"sound/bird.wav"};

/* end of sounds */

static int sound_ok = 0;
static int sound_on = 0; /* = 1 if sound should be played */
static int ear_x = 0;
static int ear_y = 0;

static void channel_set_pos(int channel, int x, int y)
{
  int panning = (x - ear_x) / 4 + 128;
  int d;
  if (panning < 0)
    panning = 0;
  else if (panning > 255)
    panning = 255;
  Mix_SetPanning(channel, 255 - panning, panning);
  d = (600 - y) / 3;
  Mix_SetDistance(channel,d);
}

void sound_move_ear(int x, int y)
{
  int i;
  ear_x = x;
  ear_y = y;
}

/* Some of this code is cleverly stolen from the
   playwav test program of SDL_mixer -- ulf*/
int sound_setup()
{
  int audio_rate = MIX_DEFAULT_FREQUENCY;
  Uint16 audio_format = MIX_DEFAULT_FORMAT;
  int audio_channels = 1;
  int i;
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, 4096) < 0) 
    {
      fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
      return -1;
    } 
  else 
    {
      Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
      printf("Opened audio at %d Hz %d bit %s\n", audio_rate,
	     (audio_format&0xFF),
	     (audio_channels > 1) ? "stereo" : "mono");
    }
  sound_ok = 1;
  sound_on = 1;
  return 0;
}

void sound_effect(sound_t *s, int xpos, int ypos)
{
  int channel;
  if (sound_on)
    {
      if (s->init_ok == 0) /* lazy loading */
	{
	  fprintf(stderr,"loading sound '%s'\n",s->filename);
	  s->wave = Mix_LoadWAV(path_to_data(s->filename));
	  if (s->wave)
	    {
	      s->init_ok = 1;
	    }
	  else
	    {
	      fprintf(stderr,"Could not open sound %s\n",s->filename);
	      s->init_ok = -1;
	    }
	}
      if (s->init_ok == 1)
	{
	  channel = Mix_PlayChannel(-1, s->wave, s->nr_loops);
	  channel_set_pos(channel,xpos,ypos);
	}
    }
}

void sound_source_set_pos(sound_source_t *source, int x, int y)
{
  if (source->sound)
    channel_set_pos(source->channel,x,y);
}

void sound_source_set_sound(sound_source_t *source, sound_t *s)
{
  int channel;
  if ((source->sound == s) || (!sound_on))
    return;
  if (s)
    {
      if (s->init_ok == 0) /* lazy loading */
	{
	  fprintf(stderr,"loading sound '%s'\n",s->filename);
	  s->wave = Mix_LoadWAV(path_to_data(s->filename));
	  if (s->wave)
	    {
	      s->init_ok = 1;
	    }
	  else
	    {
	      fprintf(stderr,"Could not open sound %s\n",s->filename);
	      s->init_ok = -1;
	    }
	}
      if (s->init_ok == 1)
	{
	  if (source->sound)
	    channel = source->channel;
	  else
	    channel = -1;
	  channel = Mix_PlayChannel(channel, s->wave, s->nr_loops);
	  source->channel = channel;
	  source->sound = s;
	}
    }
  else
    {
      if (source->sound)
	{
	  Mix_HaltChannel(source->channel);
	  source->sound = 0;
	}
    }
}

#endif
