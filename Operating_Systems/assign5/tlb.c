/*
 * STRING.c
 *
 *  Created on: Sep 6, 2017
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tlb.h"
#include "queue.h"

#define TLB_SIZE 16

typedef struct row {
  int page;
  int frame;
} ROW;

static ROW *newROW(int p, int f) {
  ROW *r = malloc(sizeof(ROW));
  if(r == 0) {
    fprintf(stderr,"out of memory\n"); exit(1);
  }
  r->page = p;
  r->frame = f;
  return r;
}

static int getRowPage(ROW *r) {return r->page;}
static int getRowFrame(ROW *r){return r->frame;}

static void displayROW(FILE *fp, void *r) {
  fprintf(fp,"[Page: %d, Frame: %d]",getRowPage(r),getRowFrame(r));
}

struct tlb {
    QUEUE *rows;
};

/** Creates a new ADDRESS object
  */
TLB *newTLB() {
    TLB *tlb = malloc(sizeof(TLB));
    if (tlb == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    tlb->rows = newQUEUE(displayROW);
    for(int x = 0; x < TLB_SIZE; x++) {
      enqueue(tlb->rows,newROW(-1,-1));
    }

    return tlb;
}

int checkTLB(TLB *tlb,int page) {
  int frame = -1;
  for(int x = 0; x < sizeQUEUE(tlb->rows); x++) {
    ROW *r = dequeue(tlb->rows);
    if(r->page == page && r->frame != -1) {
      frame = r->frame;
    }
    enqueue(tlb->rows,r);
  }
  return frame;
}

void insertTLB(TLB *tlb,int page, int frame) {
  if(sizeQUEUE(tlb->rows) == TLB_SIZE) {
    dequeue(tlb->rows);
  }
  enqueue(tlb->rows,newROW(page,frame));
}
