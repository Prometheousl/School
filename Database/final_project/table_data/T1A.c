/*
 *  table.c
 *
 *  Created on: April 7, 2018
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "T1A.h"
#include "../casting/int.h"
#include "../casting/m_string.h"

struct table_a {
  DA *K1, *TS, *C1, *C2; // columns
  char *key;
  int size;
  DA *all_columns;
};

extern TABLEA *newTABLEA() {
  TABLEA *ta = malloc(sizeof(TABLEA));
  if(ta == 0) {
    fprintf(stderr,"out of memory\n"); exit(1);
  }
  ta->K1 = newDA(displayINT);
  ta->TS = newDA(displayINT);
  ta->C1 = newDA(displayINT);
  ta->C2 = newDA(displayINT);
  ta->key = "K1 TS";
  ta->size = 0;
  ta->all_columns = newDA(displaySTRING);
  insertDA(ta->all_columns,newSTRING("K1A"));
  insertDA(ta->all_columns,newSTRING("TSA"));
  insertDA(ta->all_columns,newSTRING("C1"));
  insertDA(ta->all_columns,newSTRING("C2"));

  return ta;
}

extern DA *getAllColumnsA(TABLEA *ta) {
  return ta->all_columns;
}

extern void insertRowA(TABLEA *ta, int k1, int ts, int c1, int c2) {
  insertDA(ta->K1,newINT(k1));
  insertDA(ta->TS,newINT(ts));
  insertDA(ta->C1,newINT(c1));
  insertDA(ta->C2,newINT(c2));
  ta->size++;
}

extern int getSizeA(TABLEA *t) {
  return t->size;
}

extern DA *getK1A(TABLEA *t) {
  return t->K1;
}

extern DA *getTSA(TABLEA *t) {
  return t->TS;
}

extern DA *getC1(TABLEA *t) {
  return t->C1;
}

extern DA *getC2(TABLEA *t) {
  return t->C2;
}

extern void printTA(FILE *fp, TABLEA *ta) {
  fprintf(fp,"    K1     TS     C1     C2\n\n");
  for(int x = 0; x < ta->size; x++) {
    fprintf(fp,"%6d %6d %6d %6d\n",
      getINT(getDA(ta->K1,x)),getINT(getDA(ta->TS,x)),
      getINT(getDA(ta->C1,x)),getINT(getDA(ta->C2,x)));
  }
}
