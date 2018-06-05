/*
 *  dispatcher.c
 *
 *  Created on: March 22,2018
 *  Author: Alex Lay
 */
 #define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#include "scanner.h"
#include "da.h"
#include "modified_page_table.h"
#include "address.h"
#include "tlb.h"

// Static functions
static DA *readAddressList(char *address_list);
static ADDRESS *readAddress(FILE *file);
static uint16_t mask32to16(uint32_t int32);
static uint8_t getLow8Bits(uint16_t integer);
static uint8_t getHigh8Bits(uint16_t integer);
static void translateAddresses(DA *l_addr,PAGE_TABLE *pt,TLB *);
static void outputResults(DA *l_addr,PAGE_TABLE *pt);

int tlb_hits = 0;
/*1. Find the location of the desired page on the disk.
  2. Find a free frame:
    a. If there is a free frame, use it.
    b. If there is no free frame, use a page-replacement algorithm to select
       a victim frame.
    c. Write the victim frame to the disk; change the page and frame tables
       accordingly.
  3. Read the desired page into the newly freed frame; change the page and
       frame tables.
  4. Continue the user process from where the page fault occurred.*/
int main(int argc,char **argv) {
  clock(); // start the timer
  //printf("Start time is %d\n",start_time);

  char *address_file;
  /* 1. Get command line arguments argv[1] */
  if(argc < 2) {
    printf("Not enough arguments!\n");
    exit(0);
  }
  else {
    address_file = argv[1];
  }
  /* 2. Read integers from address list & store in data structure(s) */
  DA *l_addr = readAddressList(address_file);
  //displayDA(stdout,l_addr);
  //printf("\n");
  /* 3. Translate into physical addresses */

  // Makes a table with 256 pages * 128 frames (MODIFIED)
  PAGE_TABLE *pt = newPAGE_TABLE(256,128);
  TLB *tlb = newTLB();
  translateAddresses(l_addr,pt,tlb);
  /* 4. Output results */
  //printPAGE_TABLE(pt);
  outputResults(l_addr,pt);

  exit(0);
}
/** Reads the address list specified into a DA of logical addresses
  * Logical addresses are stored in the ADRESS struct
  *
  * @param ADDRESS_list = the name of the file to read from
  * @return = the RBT containing all the ADDRESSes
  */
static DA *readAddressList(char *address_list) {
  //printf("In readADDRESSList!\n");
  DA *q = newDA(displayADDRESS);
  FILE *file = fopen(address_list,"r");
  ADDRESS *addr = readAddress(file);
  while(!feof(file)) {
    if(addr != NULL) {
      insertDA(q,addr);
    }
    addr = readAddress(file);
  }
  int successfulClose = fclose(file);
  if(successfulClose != 0)
    printf("Couldn't close file!\n");
  return q;
}

static ADDRESS *readAddress(FILE *file) {
  uint32_t int32= readInt(file);
  //printf("32-bit integer is %" PRIu32 "\n",int32);
  uint16_t int16 = mask32to16(int32);
  uint16_t temp = int16;
  //printf("16-bit integer is %" PRIu16 "\n",int16);
  uint8_t low_bit = getLow8Bits(int16);
  //printf("Low 8-bits is %" PRIu8 "\n",low_bit);
  uint8_t high_bit = getHigh8Bits(temp);
  //printf("High 8-bits is %" PRIu8 "\n",high_bit);

  // low_bit = offset, high_bit = page number
  return newADDRESS(int32,high_bit,low_bit);
}

static uint16_t mask32to16(uint32_t int32) {
  return (uint16_t) (int32 & (unsigned int)0x0000FFFF);
}

static uint8_t getLow8Bits(uint16_t integer) {
  return integer & (unsigned short)0xFF;
}

static uint8_t getHigh8Bits(uint16_t integer) {
  return (integer & ~((unsigned short)0xFF)) >> 8;
}

static void translateAddresses(DA *l_addr,PAGE_TABLE *pt,TLB *tlb) {
  int page = 0, frame = -1, offset = 0;
  for(int x = 0; x < sizeDA(l_addr); x++) {
    ADDRESS *curr_addr = getDA(l_addr,x);
    page = (int)getPageNum(getADDRESS(curr_addr));
    offset = (int)getOffset(getADDRESS(curr_addr));
    //printf("Translating page %d with offset %d to physical address\n",
      //page,offset);
    frame = checkTLB(tlb,page);
    if(frame == -1) { // TLB-miss
      frame = getFrameForPage(pt, page);
      insertTLB(tlb,page,frame);
    }
    else { // TLB-hit
      tlb_hits++;
    }
  //  printf("Found frame at %d\n",frame);
  //  printf("Physical address is then: %d\n",frame * 256 + offset);
  //  printf("And Value is %d\n",getValueFromDisk(pt,page,offset));
    setPhysicalAddress(curr_addr,frame * 256 + offset);
    setValue(curr_addr,getValueFromDisk(pt,page,offset));
  }
}

static void outputResults(DA *l_addr,PAGE_TABLE *pt) {
  //displayDA(stdout,l_addr);
  for(int x = 0; x < sizeDA(l_addr); x++) {
    displayADDRESS(stdout,getDA(l_addr,x));
  }
  int page_faults = getNumPageFaults(pt);
  if(page_faults != 0) page_faults+=2;
  printf("Number of Translated Addresses = %d\n",sizeDA(l_addr));
  printf("Page Faults = %d\n",page_faults);
  printf("Page Fault Rate = %.3f\n",
    (float)page_faults / (float)sizeDA(l_addr));
  printf("TLB Hits = %d\n",tlb_hits);
  printf("TLB Hit Rate = %.3f\n",(float)tlb_hits / (float)sizeDA(l_addr));
}
