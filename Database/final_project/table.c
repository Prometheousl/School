/*
 *  table.c
 *
 *  Created on: April 7, 2018
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "table.h"
#include "casting/m_string.h"
#include "casting/real.h"
#include "decision_tree.h"

struct row {
  DA *row;
};

extern ROW *newROW(DA *values) {
  ROW *r = malloc(sizeof(ROW));
  if(r == 0) {
    fprintf(stderr,"out of memory\n"); exit(1);
  }
  r->row = values;

  return r;
}
extern ROW *getROW(void *r) {
  return (ROW *)r;
}
extern DA *getROWdata(ROW *r) {
  return r->row;
}

extern void displayROW(FILE *fp,void *row) {
  ROW *r = getROW(row);
  int row_size = sizeDA(r->row);
  for(int x = 0; x < row_size; x++) {
    if(getDA(r->row,x) == NULL || getINT(getDA(r->row,x)) == -1)
      fprintf(fp,"%8s","");
    else if(fabs(getREAL(getDA(r->row,x))) > 10e-7) // floating-point comparison is weird!
      fprintf(fp,"%8.2f\n",getREAL(getDA(r->row,x)));
    else
      fprintf(fp,"%8d",getINT(getDA(r->row,x)));
  }
  fprintf(fp,"\n");
}

struct table {
  DA *columns; // can contain aggregates
  DA *rows;
  int size;
  char *key;
};

extern TABLE *newTABLE() {
  TABLE *t = malloc(sizeof(TABLE));
  if(t == 0) {
    fprintf(stderr,"out of memory\n"); exit(1);
  }
  t->columns = newDA(displaySTRING);
  t->rows = newDA(displayROW);
  t->size = 0;

  return t;
}

extern TABLE *getTABLE(void *t) {
  return (TABLE *)t;
}

extern int getSize(TABLE *t) {
  return sizeDA(t->rows);
}

extern void setColumns(TABLE *t,DA *c) {
  t->columns = c;
}

extern DA *getTableColumns(TABLE *t) {
  return t->columns;
}

extern void insertRow(TABLE *t,DA *values) {
  ROW *r = newROW(values);
  insertDA(t->rows,r);
  t->size++;
}

extern ROW *getROWatIndex(TABLE *t,int index) {
  return getDA(t->rows,index);
}

static ROW *combineRows(ROW *r1,ROW *r2) {
  DA *new_row = newDA(displayINT);
  DA *row1 = getROWdata(r1);
  DA *row2 = getROWdata(r2);
  for(int x = 0; x < sizeDA(row1); x++) {
    insertDA(new_row,getDA(row1,x));
  }
  for(int x = 0; x < sizeDA(row2); x++) {
    insertDA(new_row,getDA(row2,x));
  }
  return newROW(new_row);
}

extern INTEGER *getTableData(TABLE *t,char *col_name,int row_index) {
  int column_index = 0;
  for(int x = 0; x < sizeDA(t->columns); x++) {
    //printf("Comparing %s with %s\n",getSTRING(getDA(t->columns,x)),col_name);
    if(strcmp(getSTRING(getDA(t->columns,x)),col_name) == 0) {
      //printf("They are equal! Column index is %d.\n",x);
      column_index = x;
    }
    else if(strcmp(getSTRING(getDA(t->columns,x)),"K1A") == 0 &&
            strcmp(col_name,"K1") == 0) {
              column_index = x;
            }
    else if(strcmp(getSTRING(getDA(t->columns,x)),"K1B") == 0 &&
            strcmp(col_name,"K1") == 0) {
              column_index = x;
            }
  }
  return getDA(getROWdata(getDA(t->rows,row_index)),column_index);
}

extern void setRows(TABLE *t,DA *rs) {
  t->rows = rs;
}

extern DA *getRows(TABLE *t) {
  return t->rows;
}

extern void setKey(TABLE *t,char *k) {
  t->key = k;
}

static DA *combineTABLEcolumns(TABLE *t1,TABLE *t2) {
  DA *new_columns = newDA(displaySTRING);
  DA *t1_cols = getTableColumns(t1);
  DA *t2_cols = getTableColumns(t2);
  for(int x = 0; x < sizeDA(t1_cols); x++) {
    insertDA(new_columns,getDA(t1_cols,x));
  }
  for(int x = 0; x < sizeDA(t2_cols); x++) {
    insertDA(new_columns,getDA(t2_cols,x));
  }
  return new_columns;
}

static int satisfiesOnCondition(QUEUE *postfix_queue,DA *columns,ROW *r) {
  //printf("Testing on clause\n");
  if(sizeQUEUE(postfix_queue) == 0) return 0;
  DA *values = getROWdata(r);
  return calculatePostfix(postfix_queue,columns,values);
}

static TABLE *combineTABLES(TABLE *t1, TABLE *t2) {
  TABLE *new_table = newTABLE();
  setColumns(new_table,combineTABLEcolumns(t1,t2));
  //displayDA(stdout,new_table->columns);
  //printf("\n");
  DA *row = NULL;

  for(int x = 0; x < t1->size; x++) {
    row = newDA(displayINT);
    // insert t1 row values
    for(int y = 0; y < sizeDA(new_table->columns); y++) {
      if(y < sizeDA(t1->columns))
        insertDA(row,getDA(getROWdata(getDA(t1->rows,x)),y));
      else
        insertDA(row,newINT(-1));
    }
    /*printf("Inserting row: ");
    displayDA(stdout,row);
    printf("\n");*/
    insertRow(new_table,row);
  }

  /*printf("Table 2 is:\n ");
  displayTABLE(stdout,t2);
  printf("\n");*/
  for(int w = 0; w < t2->size; w++) {
    row = newDA(displayINT);
    // insert t2 row values
    for(int z = 0; z < sizeDA(new_table->columns); z++) {
      //printf("At column index %d: ",z);
      if(z < sizeDA(t1->columns)) {
        //printf("Inserting -1\n");
        insertDA(row,newINT(-1));
      }
      else {
        INTEGER *new_int = getDA(getROWdata(getDA(t2->rows,w)),z  - sizeDA(t1->columns));
        /*if(new_int == NULL)
          printf("What the fuck\n");
        else
          printf("Inserting %d into row\n",getINT(new_int));*/
        insertDA(row,new_int);
      }
    }
    /*printf("Inserting row: ");
    displayDA(stdout,row);
    printf("\n");*/
    insertRow(new_table,row);
  }
  return new_table;
}

extern TABLE *combineTABLESOn(TABLE *t1, TABLE *t2,QUEUE *postfix_queue) {
  printf("Combining tables. On postfix queue is: ");
  displayQUEUE(stdout,postfix_queue);
  printf("\n");
  if(postfix_queue == NULL || sizeQUEUE(postfix_queue) == 0)
    return combineTABLES(t1,t2);
  TABLE *new_table = newTABLE();
  setColumns(new_table,combineTABLEcolumns(t1,t2));
  displayDA(stdout,new_table->columns);
  printf("\n");
  //int size = (t1->size > t2->size ? t1->size : t2->size);
  //DA *row = NULL;

  for(int x = 0; x < t1->size; x++) {
    //row = newDA(displayINT);
    for(int y = 0; y < t2->size; y++) {
      ROW *t1_row = getROWatIndex(t1,x);
      ROW *t2_row = getROWatIndex(t2,y);
      ROW *combined_row = combineRows(t1_row,t2_row);
      if(satisfiesOnCondition(postfix_queue,new_table->columns,combined_row)) {
        insertRow(new_table,getROWdata(combined_row));
      }
    }
  }

  // for every row in the larger table
  /*for(int x = 0; x < size; x++) {
    row = newDA(displayINT);
    // insert t1 row values & then t2 values
    for(int y = 0; y < sizeDA(getTableColumns(t1)); y++) {
      if(x >= t1->size)
        insertDA(row,newINT(-1));
      else
        insertDA(row,getDA(getROWdata(getDA(t1->rows,x)),y));
    }
    for(int y = 0; y < sizeDA(getTableColumns(t2)); y++) {
      if(x >= t2->size)
        insertDA(row,newINT(-1));
      else
        insertDA(row,getDA(getROWdata(getDA(t2->rows,x)),y));
    }
    if(satisfiesOnCondition(postfix_queue,new_table->columns,newROW(row))) {
      printf("Inserting row: ");
      displayDA(stdout,row);
      printf("\n");
      insertRow(new_table,row);
    }
  }*/
  return new_table;
}

extern TABLE *combineTABLESAandB(TABLE *ta,TABLE *tb) {
  printf("Combining tables A and B!\n");
  TABLE *new_table = newTABLE();
  setColumns(new_table,combineTABLEcolumns(ta,tb));
  DA *row = NULL;
  DA *marked_1 = newDA(displayINT);
  DA *marked_2 = newDA(displayINT);
  DA *combined_row = NULL;
  DA *combined_rows = newDA(displayROW);
  for(int x = 0; x < sizeDA(ta->rows); x++)
    insertDA(marked_1,newINT(0));
  for(int x = 0; x < sizeDA(tb->rows); x++)
    insertDA(marked_2,newINT(0));
  int size = (sizeDA(ta->rows) > sizeDA(tb->rows) ? size = sizeDA(ta->rows) : sizeDA(tb->rows));
  // if K1 & TS match, combine. Else don't combine.
  for(int x = 0; x < sizeDA(ta->rows); x++) {
    for(int w = 0; w < sizeDA(tb->rows); w++) {
      int k1a = getINT(getTableData(ta,"K1A",x));
      int k1b = getINT(getTableData(tb,"K1B",w));
      int tsa = getINT(getTableData(ta,"TSA",x));
      int tsb = getINT(getTableData(tb,"TSB",w));
      printf("K1A is %d\n",k1a);
      printf("K1B is %d\n",k1b);
      printf("TSA is %d\n",tsa);
      printf("TSB is %d\n",tsb);
      if(k1a == k1b && tsa == tsb) { // combine rows
        combined_row = newDA(displayINT);
        setDA(marked_1,x,newINT(1));
        setDA(marked_2,w,newINT(1));
        printf("Combining rows!\n");
        for(int y = 0; y < sizeDA(ta->columns); y++)
          insertDA(combined_row,getDA(getROWdata(getDA(ta->rows,x)),y));
        for(int z = 0; z < sizeDA(tb->columns); z++)
          insertDA(combined_row,getDA(getROWdata(getDA(tb->rows,w)),z));
        insertDA(combined_rows,newROW(combined_row));
      }
      else { // insert column by itself
        printf("not combining rows.\n");
      }
    }
  }
  printf("After combining rows. Rows are:\n");
  for(int x = 0; x < sizeDA(combined_rows); x++) {
    displayROW(stdout,getDA(combined_rows,x));
    printf("\n");
  }
  // insert uncombined T1 values
  for(int x = 0; x < sizeDA(ta->rows); x++) {
    row = newDA(displayINT);
    if(getINT(getDA(marked_1,x)) == 0) {
      for(int y = 0; y < sizeDA(new_table->columns); y++) {
        if(y < sizeDA(ta->columns))
          insertDA(row,getDA(getROWdata(getDA(ta->rows,x)),y));
        else
          insertDA(row,newINT(-1));
      }
      insertRow(new_table,row);
    }
  }
  // insert combined values
  for(int x = 0; x < sizeDA(combined_rows); x++) {
    insertRow(new_table,getROWdata(getDA(combined_rows,x)));
  }
  // insert uncombined T2 values
  for(int x = 0; x < sizeDA(tb->rows); x++) {
    row = newDA(displayINT);
    if(getINT(getDA(marked_2,x)) == 0) {
      printf("not combined at %d\n",x);
      printf("Row at x is: ");
      displayROW(stdout,getDA(tb->rows,x));
      for(int y = 0; y < sizeDA(new_table->columns); y++) {
        if(y >= sizeDA(ta->columns))
          insertDA(row,getDA(getROWdata(getDA(tb->rows,x)),y - sizeDA(ta->columns)));
        else
          insertDA(row,newINT(-1));
      }
      insertRow(new_table,row);
    }
  }

  return new_table;
}

extern void displayTABLE(FILE *fp,void *table) {
  TABLE *t = getTABLE(table);
  char *col_name = NULL;
  for(int x = 0; x < sizeDA(t->columns); x++) {
    col_name = getSTRING(getDA(t->columns,x));
    if(strcmp(col_name,"K1A") == 0)
      col_name = "K1";
    else if(strcmp(col_name,"K1B") == 0)
      col_name = "K1";
    else if(strcmp(col_name,"TSA") == 0)
      col_name = "TS";
    else if(strcmp(col_name,"TS2") == 0 || strcmp(col_name,"TSB") == 0)
      col_name = "TS";
    if(x != sizeDA(t->columns) - 1)
      fprintf(fp,"%8s",col_name);
    else
      fprintf(fp,"%8s\n",col_name);
  }

  for(int x = 0; x < sizeDA(t->columns); x++) {
    fprintf(fp,"  ------");
  }
  fprintf(fp,"\n");
  if(t->rows != NULL) {
    for(int x = 0; x < sizeDA(t->rows); x++) {
      displayROW(fp,getDA(t->rows,x));
    }
  }
  fprintf(fp,"\n");
}

extern void generateInsertStatements(FILE *fp,TABLE *t) {
  ROW *row = NULL;
  DA *r = NULL;
  char *table_name = NULL;
  if(sizeDA(t->columns) == 9)
    table_name = "t1";
  else if(strcmp(getSTRING(getDA(t->columns,2)),"C1") == 0)
    table_name = "t1a";
  else if(strcmp(getSTRING(getDA(t->columns,2)),"C3") == 0)
    table_name = "t1b";
  else
    table_name = "t2";
  for(int x = 0; x < t->size; x++) {
    row = getROWatIndex(t,x);
    r = getROWdata(row);
    fprintf(fp,"INSERT INTO %s VALUES(",table_name);
    for(int y = 0; y < sizeDA(r); y++) {
      if(y != sizeDA(r) - 1)
        fprintf(fp,"%d,",getINT(getDA(r,y)));
      else
        fprintf(fp,"%d);\n",getINT(getDA(r,y)));
    }
  }
}
