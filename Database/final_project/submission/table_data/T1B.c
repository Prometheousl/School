/*
 *  table.c
 *
 *  Created on: April 7, 2018
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "T1B.h"
#include "../casting/int.h"
#include "../casting/m_string.h"


struct table_b {
  DA *K1, *TS, *C3, *C4, *C5; // columns
  char *key;
  int size;
  DA *all_columns;
};

extern TABLEB *newTABLEB() {
  TABLEB *tb = malloc(sizeof(TABLEB));
  if(tb == 0) {
    fprintf(stderr,"out of memory\n"); exit(1);
  }
  tb->K1 = newDA(displayINT);
  tb->TS = newDA(displayINT);
  tb->C3 = newDA(displayINT);
  tb->C4 = newDA(displayINT);
  tb->C5 = newDA(displayINT);
  tb->key = "K1 TS";
  tb->size = 0;
  tb->all_columns = newDA(displaySTRING);
  insertDA(tb->all_columns,newSTRING("K1B"));
  insertDA(tb->all_columns,newSTRING("TSB"));
  insertDA(tb->all_columns,newSTRING("C3"));
  insertDA(tb->all_columns,newSTRING("C4"));
  insertDA(tb->all_columns,newSTRING("C5"));

  return tb;
}

extern DA *getAllColumnsB(TABLEB *tb) {
  return tb->all_columns;
}

extern void
insertRowB(TABLEB *tb, int k1, int ts, int c3, int c4, int c5) {
  insertDA(tb->K1,newINT(k1));
  insertDA(tb->TS,newINT(ts));
  insertDA(tb->C3,newINT(c3));
  insertDA(tb->C4,newINT(c4));
  insertDA(tb->C5,newINT(c5));

  tb->size++;
}

extern int getSizeB(TABLEB *t) {
  return t->size;
}

extern DA *getK1B(TABLEB *t) {
  return t->K1;
}

extern DA *getTSB(TABLEB *t) {
  return t->TS;
}

extern DA *getC3(TABLEB *t) {
  return t->C3;
}

extern DA *getC4(TABLEB *t) {
  return t->C4;
}

extern DA *getC5(TABLEB *t) {
  return t->C5;
}

extern void printTB(FILE *fp, TABLEB *tb) {
  fprintf(fp,"    K1     TS     C3     C4     C5\n\n");
  for(int x = 0; x < tb->size; x++) {
    fprintf(fp,"%6d %6d %6d %6d %6d\n",
      getINT(getDA(tb->K1,x)),getINT(getDA(tb->TS,x)),
      getINT(getDA(tb->C3,x)),getINT(getDA(tb->C4,x)),
      getINT(getDA(tb->C5,x)));
  }
}
