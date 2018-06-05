/*
 *  table.c
 *
 *  Created on: April 7, 2018
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "T2.h"
#include "../casting/int.h"

struct table_2 {
  DA *K2, *TS2, *A1, *A2, *A3; // columns
  char *key;
  int size;
};

extern TABLE2 *newTABLE2() {
  TABLE2 *t = malloc(sizeof(TABLE2));
  if(t == 0) {
    fprintf(stderr,"out of memory\n"); exit(1);
  }
  t->K2 = newDA(displayINT);
  t->TS2 = newDA(displayINT);
  t->A1 = newDA(displayINT);
  t->A2 = newDA(displayINT);
  t->A3 = newDA(displayINT);
  t->key = "K2 TS2";
  t->size = 0;

  return t;
}

extern void
insertRow2(TABLE2 *t, int k1, int ts, int c3, int c4, int c5) {
  insertDA(t->K2,newINT(k1));
  insertDA(t->TS2,newINT(ts));
  insertDA(t->A1,newINT(c3));
  insertDA(t->A2,newINT(c4));
  insertDA(t->A3,newINT(c5));

  t->size++;
}

extern int getSize2(TABLE2 *t) {
  return t->size;
}

extern DA *getK2(TABLE2 *t) {
  return t->K2;
}

extern DA *getTS2(TABLE2 *t) {
  return t->TS2;
}

extern DA *getA1(TABLE2 *t) {
  return t->A1;
}

extern DA *getA2(TABLE2 *t) {
  return t->A2;
}

extern DA *getA3(TABLE2 *t) {
  return t->A3;
}

extern void printT2(FILE *fp, TABLE2 *t) {
  fprintf(fp,"    K2     TS2     A1     A2     A3\n\n");
  for(int x = 0; x < t->size; x++) {
    fprintf(fp,"%6d %6d %6d %6d %6d\n",
      getINT(getDA(t->K2,x)),getINT(getDA(t->TS2,x)),
      getINT(getDA(t->A1,x)),getINT(getDA(t->A2,x)),
      getINT(getDA(t->A3,x)));
  }
}
