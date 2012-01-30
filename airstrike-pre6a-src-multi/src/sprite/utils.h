#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>

#define CRITICAL(pred) do {if (!(pred)) { fprintf(stderr,"Critical error at %s:%i. Please send a bug report.\n",__FILE__,__LINE__); exit(EXIT_FAILURE);}} while (0)

#endif
