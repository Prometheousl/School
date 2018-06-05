#ifndef __SET_INCLUDED__
#define __SET_INCLUDED__

#include <stdio.h>

typedef struct set SET;

extern SET *newSET(void (*d)(FILE *,void *));
extern int makeSET(SET *d,void *value);
extern int findSET(SET *d,int index);
extern int unionSET(SET *d,int index1,int index2);
extern int countSET(SET *d);
extern int displaySET(FILE *,SET *d);

#endif