/*
 * PAGE_TABLE.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef PAGE_TABLE_H_
#define PAGE_TABLE_H_

#include <stdio.h>
#include <stdint.h>
#include "entry.h"

typedef struct page_table PAGE_TABLE;

extern PAGE_TABLE *newPAGE_TABLE(int,int);
int getNumPageFaults(PAGE_TABLE *pt);
extern int getFrameForPage(PAGE_TABLE *pt, int page);
extern void printPAGE_TABLE(PAGE_TABLE *pt);
extern ENTRY *getEntry(PAGE_TABLE *pt,int page);
extern signed int getValueFromDisk(PAGE_TABLE *pt,int page,int offset);


#endif /* STRING_H_ */
