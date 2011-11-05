#ifndef CONFIG_H
#define CONFIG_H

#define VERSIONSTRING "Airstrike 1.0 pre6"

/* absolute path to main config file (for now it's airstrikerc in the
   current dir)*/
#ifndef ROOT_CONFIG_FILE
#define ROOT_CONFIG_FILE "airstrikerc"
#endif
/* path to file in users $HOME */
#ifndef USER_CONFIG_FILE
#define USER_CONFIG_FILE ".airstrikerc"
#endif

#ifdef USE_SOUND
#define SDL_INITFLAGS SDL_INIT_VIDEO|SDL_INIT_AUDIO
#else
#define SDL_INITFLAGS SDL_INIT_VIDEO
#endif

int config_setup();

float cfgnum(char *name, float defaul);
char *cfgstr(char *name, char *defaul);

/*
 * Returns a full path where file is the path relative to
 * the data root. String is only valid until the next call 
 * to this fn. (ie. not reentrant)
 */
char *path_to_data(char *file);

#endif
