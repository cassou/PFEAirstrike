#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "names.h"

struct names
{
  char *name;
  void *value;
  struct names *next;
};

void *nget(struct names *names, const char *name)
{
  while (names)
    {
      if (strcmp(name,names->name) == 0)
	return names->value;
      else
	names = names->next;
    }
  /* For the moment we fail here to catch bugs */
  /*fprintf(stderr,"nget(): Did not find '%s'\n",name);*/
  return 0;
}

int nhas(struct names *names, const char *name)
{
  while (names)
    {
      if (strcmp(name,names->name) == 0)
	return 1;
      else
	names = names->next;
    }
  return 0;
}

void ndef(struct names **names, const char *name, void *value)
{
  struct names *n = malloc(sizeof(*n));
  n->next = *names;
  n->value = value;
  n->name = malloc(strlen(name) + 1);
  strcpy(n->name,name);
  *names = n;
}

void nundef(struct names **names, const char *name)
{
  struct names *n;
  while (*names)
    {
      if (strcmp(name,(*names)->name) == 0)
	{
	  n = *names;
	  *names = (*names)->next;
	  free(n->name);
	  free(n);
	  return;
	}
      else
	{
	  names = &((*names)->next);
	}
    }
}

void nmap(struct names *names, void (*fn)(const char *name, void *value))
{
  while (names)
    {
      fn(names->name,names->value);
      names = names->next;
    }
}

void nfree(struct names **names)
{
  struct names *n;
  while (*names)
    {
      n = *names;
      *names = (*names)->next;
      free(n->name);
      free(n);
    }
}

void nwrite_str(struct names *names, FILE *target)
{
   while (names)
    {
      fprintf(target,"\"%s\" \"%s\"\n",names->name,(char *)names->value);
      names = names->next;
    }
}

static void skip_ws(FILE *f)
{
  int c;
  while (isspace(c = fgetc(f)) && (c != EOF)) {}
  ungetc(c,f);
}

/* 
 * Skips #..., whitespace and respects double quotes 
 * Returns NULL at EOF or error
 */
static char *next_token(FILE *f)
{
  int c;
  static char token[256];
  char *s;
  
 again:
  skip_ws(f);
  c = fgetc(f);
  ungetc(c,f);
  if (c == '"')  
    {
      if (fscanf(f,"\"%[^\"\n]\"",token) != 1)
	return 0;
    }
  else if (c == '#')
    {
      if (fscanf(f,"%*[^\n]") != 0)
	return 0;
      goto again;
    }
  else
    {
      if (fscanf(f,"%[^# \n\t={}]",token) != 1)
	return 0;
    }
  s = malloc(strlen(token) + 1);
  strcpy(s,token);
  return s;
}

void nread_str(struct names **names, FILE *target)
{
  char *name,*value;
  while ((name = next_token(target)))
    {
      value = next_token(target);
      if (!value)
	{
	  fprintf(stderr,"nread(): Invalid input at end of file.\n");
	  return;
	}
      ndef(names,name,value);
      free(name);
    }
}

