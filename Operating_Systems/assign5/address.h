/*
 * ADDRESS.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef ADDRESS_H_
#define ADDRESS_H_

#include <stdio.h>
#include <stdint.h>

typedef struct address ADDRESS;

extern ADDRESS *newADDRESS(uint32_t,uint8_t,uint8_t);
extern ADDRESS *getADDRESS(ADDRESS *);
extern uint8_t getPageNum(ADDRESS *);
extern uint8_t getOffset(ADDRESS *addr);
void setPhysicalAddress(ADDRESS *addr,int pa);
int getPhysicalAddress(ADDRESS *addr);
void setValue(ADDRESS *addr,int v);
int getValue(ADDRESS *addr);
void setLogicalAddress(ADDRESS *addr,uint32_t la);
uint32_t getLogicalAddress(ADDRESS *addr);

extern void displayADDRESS(FILE *,void *);

#endif /* STRING_H_ */
