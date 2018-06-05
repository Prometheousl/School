/*
 * STRING.c
 *
 *  Created on: Sep 6, 2017
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "entry.h"

struct entry {
    int frame;
    int valid_invalid;

    signed char *page;
};

/** Creates a new ADDRESS object
  */
ENTRY *
newENTRY(int f, int v) {
    ENTRY *e = malloc(sizeof(ENTRY));
    if (e == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    e->frame = f;
    e->valid_invalid = v;
    e->page = NULL;

    return e;
}

extern ENTRY *getENTRY(ENTRY *e) {
  return e;
}

extern void setENTRY(ENTRY *e,int f, signed char *p) {
  e->frame = f;
  e->page = p;
  e->valid_invalid = 1;
}

extern signed char *getPage(ENTRY *e) {
  return e->page;
}

extern void setPage(ENTRY *e,signed char *p) {
  e->page = p;
}

extern void setFrame(ENTRY *e,int f) {
  e->frame = f;
}

int getFrame(ENTRY *e) {
  return e->frame;
}
int isValid(ENTRY *e) {
  return e->valid_invalid;
}
void setValid(ENTRY *e, int v) {
  e->valid_invalid = v;
}
/** Displays the ADDRESS object
  *
  * @param fp = the file to display to
  * @param p = The ADDRESS object
  */
void
displayENTRY(FILE *fp,void *e) {
  char i_v = 'i';
  if(isValid(e)) i_v = 'v';
  fprintf(fp,"[%d,%c]->%s",getFrame(e),i_v,getPage(e));
}
