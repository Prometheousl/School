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
#include <time.h>
#include "modified_entry.h"

struct entry {
    int frame;
    int page_num;
    int valid_invalid;
    signed char *page;

    int time_last_accessed;
};

/** Creates a new ADDRESS object
  */
ENTRY *
newENTRY(int f, int v, int p_num) {
    ENTRY *e = malloc(sizeof(ENTRY));
    if (e == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    e->frame = f;
    e->valid_invalid = v;
    e->page_num = p_num;
    e->page = NULL;
    //e->time_last_accessed = clock();

    return e;
}

extern ENTRY *getENTRY(ENTRY *e) {
  return e;
}

extern void setPageNumber(ENTRY *e, int p_num) {
  e->page_num = p_num;
}

extern int getPageNumber(ENTRY *e) {
  return e->page_num;
}

extern void setTimeLastAccessed(ENTRY *e,int time) {
  e->time_last_accessed = time;
}

extern int getTimeLastAccessed(ENTRY *e) {
  return e->time_last_accessed;
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
  int time = clock();
  //printf("Setting time last accessed to %d\n",time);
  setTimeLastAccessed(e,time);
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
