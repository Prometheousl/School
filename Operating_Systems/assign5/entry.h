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

extern ENTRY *newENTRY(int,int);
extern ENTRY *getENTRY(ENTRY *);
extern void setENTRY(ENTRY *,int,signed char *);
extern signed char *getPage(ENTRY *);
extern void setPage(ENTRY *,signed char *);
extern void setFrame(ENTRY *,int);
extern int getFrame(ENTRY *);
extern int isValid(ENTRY *);
void setValid(ENTRY *m, int f);
extern void displayENTRY(FILE *,void *);

#endif /* STRING_H_ */
