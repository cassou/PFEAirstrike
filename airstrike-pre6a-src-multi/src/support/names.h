#ifndef NAMES_H
#define NAMES_H

#include <stdio.h>

/* A very simple 'namespace' to store pointers by name
 * Use by defining a pointer like
 * struct names *mybindings = 0;
 * And use ndef to define new names.
 * Not very fast, but easy to use and
 * convenient.
 */

struct names; 

/* returns 0 if the name was not found */
void *nget(struct names *names, const char *name);
void ndef(struct names **names, const char *name, void *value);
void nundef(struct names **names, const char *name);
int nhas(struct names *names, const char *name);
/* executes fn for each name-value pair */
void nmap(struct names *names, void (*fn)(const char *name, void *value));
void nfree(struct names **names);

/* Assume the value of each name is a string,
 * and write all the (name,value) pairs to file
 */
void nwrite_str(struct names *names, FILE *target);
/* Read from file, skipping #-comments */
void nread_str(struct names **names, FILE *target);

#endif
