#include <string.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "config.h"
#include "names.h"

static struct names *config_user = 0;
static char *datapath;

int config_setup()
{
  FILE *cfile;
  char cbuf[1024]; /* stupid, really */
  int got_file = 0;
  cfile = fopen(ROOT_CONFIG_FILE,"r");
  if (cfile)
    {
      nread_str(&config_user,cfile);
      fclose(cfile);
      got_file = 1;
    }
  if (getenv("HOME"))
    {
      sprintf(cbuf,"%s/%s",getenv("HOME"),USER_CONFIG_FILE);      
      cfile = fopen(cbuf,"r");
      if (cfile)
	{
	  nread_str(&config_user,cfile);
	  fclose(cfile);
	  got_file = 1;
	}
    }
  if (!got_file)
    {
      fprintf(stderr,"Looked for config files at '%s' and '$HOME/%s', but could not find any. Quitting.\n",ROOT_CONFIG_FILE,USER_CONFIG_FILE);
      exit(EXIT_FAILURE);
    }
  datapath = cfgstr("datapath",0);
  if (!datapath) 
    { 
      fprintf(stderr,"Cannot find datapath in config files. Quitting.\n");
      exit(EXIT_FAILURE);
    }
  return 0;
}

float cfgnum(char *name, float defaul)
{
  char *s = nget(config_user,name);
  int i;
  float f;
  if (!s)
    {
      fprintf(stderr,"In config file: Could not find '%s'. Using default (%f).\n",name,defaul);
      return defaul;
    }
  i = sscanf(s,"%f",&f);
  if (i != 1)
    {
      fprintf(stderr,"In config file: Expected number for name '%s', found '%s'. Using default (%f).\n",name,s,defaul);
      return defaul;
    }
  return f;
}

char *cfgstr(char *name, char *defaul)
{
  char *s = nget(config_user,name);
  if (s)
    return s;
  else
    return defaul;
}

char *path_to_data(char *file)
{
  static char path[512];
  sprintf(path,"%s/%s",datapath,file);
  return path;
}
