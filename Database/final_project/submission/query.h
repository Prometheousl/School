/*
 * File: table.h
 *
 * Author: Alex Lay
 * Created on: April 7, 2018
 */

#ifndef QUERY_H_
#define QUERY_H_

#include <stdio.h>
#include "data_structures/da.h"
#include "data_structures/queue.h"

typedef struct query QUERY;

extern QUERY *newQUERY();
extern QUERY *getQUERY(void *q);
extern void addColumn(QUERY *q,char *c);
extern void setTable(QUERY *q,char *t);
extern void setTable2(QUERY *q,char *t);
extern void setGroupBy(QUERY *q,char *c);
extern void displayQUERY(FILE *fp,void *query);

extern QUEUE *getWherePostfixQueue(QUERY *q);
extern QUEUE *getOnPostfixQueue(QUERY *q);
extern void setWherePostfixQueue(QUERY *q,QUEUE *wpq);
extern void setOnPostfixQueue(QUERY *q,QUEUE *opq);
extern void setWhereInfixQueue(QUERY *q,QUEUE *wiq);
extern void setOnInfixQueue(QUERY *q,QUEUE *oiq);
extern void setAggregate(QUERY *q,char *a);
extern char *getAggregate(QUERY *q);

extern DA *getColumns(QUERY *q);
extern char *getTable1(QUERY *q);
extern char *getTable2(QUERY *q);
extern char *getGroupBy(QUERY *q);

#endif /* TABLE_H_ */
