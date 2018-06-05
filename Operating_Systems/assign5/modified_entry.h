/*
 * ENTRY.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef ENTRY_H_
#define ENTRY_H_

#include <stdio.h>
#include <stdint.h>

typedef struct entry ENTRY;

extern ENTRY *newENTRY(int,int,int);
extern ENTRY *getENTRY(ENTRY *);
extern void setENTRY(ENTRY *,int,signed char *);
extern int getPageNumber(ENTRY *);
extern void setPageNumber(ENTRY *,int);
extern void setFrame(ENTRY *,int);
extern int getFrame(ENTRY *);
extern signed char *getPage(ENTRY *e);
extern void setPage(ENTRY *e,signed char *p);
extern int isValid(ENTRY *);
void setValid(ENTRY *m, int f);
extern void displayENTRY(FILE *,void *);

extern void setTimeLastAccessed(ENTRY *e,int time);
extern int getTimeLastAccessed(ENTRY *e);

#endif /* STRING_H_ */
