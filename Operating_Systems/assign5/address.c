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
#include "address.h"

struct address {
    uint32_t logical_address;
    uint8_t page_number;
    uint8_t offset;
    int physical_address;
    int value;
};

/** Creates a new ADDRESS object
  */
ADDRESS *
newADDRESS(uint32_t int32,uint8_t p_num,uint8_t offs) {
    ADDRESS *addr = malloc(sizeof(ADDRESS));
    if (addr == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    addr->logical_address = int32;
    addr->page_number = p_num;
    addr->offset = offs;
    addr->physical_address = 0;
    addr->value = 0;

    return addr;
}

extern ADDRESS *getADDRESS(ADDRESS *addr) {
  return addr;
}

uint8_t getPageNum(ADDRESS *addr) {
  return addr->page_number;
}
uint8_t getOffset(ADDRESS *addr) {
  return addr->offset;
}
void setPhysicalAddress(ADDRESS *addr,int pa) {
  addr->physical_address = pa;
}
int getPhysicalAddress(ADDRESS *addr) {
  return addr->physical_address;
}
void setValue(ADDRESS *addr,int v) {
  addr->value = v;
}
int getValue(ADDRESS *addr) {
  return addr->value;
}
void setLogicalAddress(ADDRESS *addr,uint32_t la) {
  addr->logical_address = la;
}
uint32_t getLogicalAddress(ADDRESS *addr) {
  return addr->logical_address;
}
/** Displays the ADDRESS object
  *
  * @param fp = the file to display to
  * @param p = The ADDRESS object
  */
void
displayADDRESS(FILE *fp,void *addr) {
    //fprintf(fp,"[%" PRIu8 ",%" PRIu8 "]", getPageNum(addr), getOffset(addr));
    fprintf(fp,"%" PRIu32 " %d %d\n",
      getLogicalAddress(addr),getPhysicalAddress(addr),getValue(addr));
}
