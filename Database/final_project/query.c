/*
 *  table.c
 *
 *  Created on: April 7, 2018
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "query.h"
#include "casting/m_string.h"

struct query {
  DA *columns; // can contain aggregates
  char *table;
  char *table2;
  QUEUE *where_infix_queue;
  QUEUE *on_infix_queue;
  QUEUE *where_postfix_queue;
  QUEUE *on_postfix_queue;
  char *group_by;
  char *aggregate;
};

extern QUERY *newQUERY() {
  QUERY *q = malloc(sizeof(QUERY));
  if(q == 0) {
    fprintf(stderr,"out of memory\n"); exit(1);
  }
  q->columns = newDA(displaySTRING);
  q->table = NULL;
  q->table2 = NULL;
  q->where_infix_queue = newQUEUE(displaySTRING);
  q->on_infix_queue = newQUEUE(displaySTRING);
  q->where_postfix_queue = newQUEUE(displaySTRING);
  q->on_postfix_queue = newQUEUE(displaySTRING);
  q->group_by = NULL;
  q->aggregate = NULL;

  return q;
}

extern void setAggregate(QUERY *q,char *a) {
  q->aggregate = a;
}

extern char *getAggregate(QUERY *q) {
  return q->aggregate;
}

extern DA *getColumns(QUERY *q) {
  return q->columns;
}

extern QUEUE *getWherePostfixQueue(QUERY *q) {
  return q->where_postfix_queue;
}

extern QUEUE *getOnPostfixQueue(QUERY *q) {
  return q->on_postfix_queue;
}

extern void setWherePostfixQueue(QUERY *q,QUEUE *wpq) {
  q->where_postfix_queue = wpq;
}

extern void setOnPostfixQueue(QUERY *q,QUEUE *opq) {
  q->on_postfix_queue = opq;
}

extern void setWhereInfixQueue(QUERY *q,QUEUE *wiq) {
  q->where_infix_queue = wiq;
}

extern void setOnInfixQueue(QUERY *q,QUEUE *oiq) {
  q->on_infix_queue = oiq;
}

extern char *getTable1(QUERY *q) {
  return q->table;
}

extern char *getTable2(QUERY *q) {
  return q->table2;
}

extern char *getGroupBy(QUERY *q) {
  return q->group_by;
}

extern QUERY *getQUERY(void *q) {
  return (QUERY *)q;
}

extern void addColumn(QUERY *q,char *c) {
  STRING *s = newSTRING(c);
  insertDA(q->columns,s);
}

extern void setTable(QUERY *q,char *t) {
  q->table = t;
}

extern void setTable2(QUERY *q,char *t) {
  q->table2 = t;
}

extern void setGroupBy(QUERY *q,char *c) {
  q->group_by = c;
}

static void displayClause(FILE *fp,QUEUE *infix_q) {
  QUEUE *temp = copyQUEUE(infix_q);
  STRING *s = NULL;
  int size = sizeQUEUE(infix_q);
  for(int x = 0; x < size; x++) {
    s = dequeue(temp);
    if(strcmp(getSTRING(s),"AND") == 0 || strcmp(getSTRING(s),"OR") == 0)
      fprintf(fp," %s ",getSTRING(s));
    else if(x != size - 1)
      fprintf(fp,"%s",getSTRING(s));
  }
}

extern void displayQUERY(FILE *fp,void *query) {
  QUERY *q = getQUERY(query);
  int num_cols = sizeDA(q->columns);
  char *condition = NULL;
  // GET col1,col2,col3
  fprintf(fp,"GET ");
  for(int x = 0; x < num_cols; x++) {
    condition = getSTRING(getDA(q->columns,x));
    if(x != num_cols - 1)
      fprintf(fp,"%s, ",condition);
    else
      fprintf(fp,"%s\n",condition);
  }
  int is_where_clause = sizeQUEUE(q->where_infix_queue);
  // FROM table [COMBINE table_2]
  if(q->table2 == NULL && !is_where_clause) {
    fprintf(fp,"FROM %s;\n",q->table);
    return;
  }
  else if(q->table2 == NULL)
    fprintf(fp,"FROM %s\n",q->table);
  else if(!is_where_clause) {
    fprintf(fp,"FROM %s COMBINE %s ON ",q->table,q->table2);
    displayClause(fp,q->on_infix_queue);
    fprintf(fp,";\n");
    return;
  }
  else {
    fprintf(fp,"FROM %s COMBINE %s ON ",q->table,q->table2);
    displayClause(fp,q->on_infix_queue);
    fprintf(fp,";\n");
  }
  // WHERE conditions
  if(is_where_clause) {
    fprintf(fp,"WHERE ");
    displayClause(fp,q->where_infix_queue);
  }
  // [GROUP BY]
  if(q->group_by == NULL)
    fprintf(fp,";\n");
  else
    fprintf(fp, "\nGROUP BY %s;\n",q->group_by);
}
