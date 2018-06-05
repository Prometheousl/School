/*
 * File: table.h
 *
 * Author: Alex Lay
 * Created on: April 7, 2018
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <stdio.h>
#include "data_structures/da.h"
#include "data_structures/queue.h"
#include "casting/int.h"

typedef struct table TABLE;
typedef struct row ROW;

extern TABLE *newTABLE();
extern TABLE *getTABLE(void *t);
extern int getSize(TABLE *t);
extern void setColumns(TABLE *t,DA *c);
extern DA *getTableColumns(TABLE *t);
extern void insertRow(TABLE *t,DA *r);
extern ROW *getROWatIndex(TABLE *t,int index);
extern INTEGER *getTableData(TABLE *t,char *col_name,int row_index);
extern void setRows(TABLE *t,DA *rs);
extern DA *getRows(TABLE *t);
extern void setKey(TABLE *t,char *k);
extern void displayTABLE(FILE *fp,void *t);
extern TABLE *combineTABLESOn(TABLE *t1,TABLE *t2,QUEUE *pstfx_q);
extern TABLE *combineTABLESAandB(TABLE *ta,TABLE *tb);
extern void generateInsertStatements(FILE *fp,TABLE *t);

extern ROW *newROW(DA *values);
extern ROW *getROW(void *r);
extern DA *getROWdata(ROW *r);
extern void displayROW(FILE *fp,void *row);

#endif /* TABLE_H_ */
