/*
 * PAGE_TABLE.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef TLB_H_
#define TLB_H_

#include <stdio.h>
#include <stdint.h>

typedef struct tlb TLB;

extern TLB *newTLB();
extern int checkTLB(TLB *tlb,int page);
void insertTLB(TLB *tlb,int page, int frame);

#endif /* STRING_H_ */
