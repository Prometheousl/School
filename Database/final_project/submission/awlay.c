/*  Created by: Alex Lay
    Date: 4/7/2018
    Class: CS557 - Database Management Systems

    Desc: Write a program in C to determine  if a table is in
      1NF, 2NF or 3NF for a given set of functional dependencies FDs,
      in other words, you have more than one functional dependency
      describing a table. Assume the table is at least in 1NF.

      Your program should be able to process more than one set of FDs,
      with the output produced after each set of FDs. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "input/scanner.h"
#include "data_structures/da.h"
#include "data_structures/queue.h"
#include "casting/int.h"
#include "casting/m_string.h"
#include "casting/real.h"
#include "query.h"
#include "table.h"
#include "decision_tree.h"
// input static functions
static void readTables(TABLE *ta,TABLE *,TABLE *);
static void readTableA(TABLE *ta,FILE *file);
static void readTableB(TABLE *tb,FILE *file);
static void readTable2(TABLE *t2,FILE *file);
static void insertRowA(TABLE *,int,int,int,int);
static void insertRowB(TABLE *,int,int,int,int,int);
static void insertRow2(TABLE *,int,int,int,int,int);
static QUEUE *readQueries();
static QUERY *readQuery(FILE *fp);
// processing queries static functions
static void processQueries(QUEUE *queries,TABLE *t1,TABLE *ta,TABLE *tb,TABLE *t2);
static TABLE *processQuery(QUERY *q,TABLE *t1,TABLE *ta,TABLE *tb,TABLE *t2);
static QUEUE *readInfixQueue(FILE *fp,STRING *token);
static void removeNonAggregate(DA *columns);
static char *findAggregateInColumns(DA *columns);
static int isAggregate(char *s);
static DA *getAllColumns(char *t1, char *t2);
static TABLE *insertRows(TABLE *t,QUERY *q,TABLE *t1,TABLE *ta,TABLE *tb,TABLE *t2);
static STRING *characterToSTRING(char c);
static void insertT1Columns(DA *columns);
static void insertT2Columns(DA *columns);
static void insertT1AColumns(DA *columns);
static void insertT1BColumns(DA *columns);
static void insertSemicolon(QUEUE *infix_queue);
static TABLE *calculateAggregates(TABLE *t,char *aggregate,char *group_by);
// misc static functions
static int isLastCharSemicolon(char *s);
static int isLastCharComma(char *s);
static char *trimLastChar(char *string);
// traversing table functions
static TABLE *traverseTable(TABLE *new_table,QUERY *q,TABLE *t);
static TABLE *traverseTableAggregate(TABLE *new_table,QUERY *q,TABLE *t);

/** Simulates running SQL queries on a database
  *
  * In the form:
  *    GET [cols]
  *    FROM [table1] [COMBINE] [table2] [ON clause]
  *    [WHERE clause]
  *    [GROUP BY col]
  *
  * This SQL-simulator supports GET,FROM,WHERE, and aggregate functions. If
  *   there are aggregate functions, an optional GROUP BY statement can be
  *   included.
  */
int main() {
  TABLE *ta = newTABLE();
  TABLE *tb = newTABLE();
  TABLE *t2 = newTABLE();
  readTables(ta,tb,t2);
  TABLE *t1 = combineTABLESAandB(ta,tb);

  QUEUE *queries = readQueries();
  processQueries(queries,t1,ta,tb,t2);
  return 0;
}
/** Reads the tables from 3 files
  *
  * @return the dynamic array of tables
  */
static void readTables(TABLE *ta,TABLE *tb,TABLE *t2) {
  FILE *file_a = fopen("T1A.txt","r");
  FILE *file_b = fopen("T1B.txt","r");
  FILE *file_2 = fopen("T2.txt","r");
  readTableA(ta,file_a);
  readTableB(tb,file_b);
  readTable2(t2,file_2);

  int unsuccessful_close_a = fclose(file_a);
  int unsuccessful_close_b = fclose(file_b);
  int unsuccessful_close_2 = fclose(file_2);
  if(unsuccessful_close_a || unsuccessful_close_b || unsuccessful_close_2)
    printf("Couldn't close file!\n");
}
/** Reads tableA from the given file
  *
  * @param file = the file to read from
  */
static void readTableA(TABLE *ta,FILE *file) {
   char *line = NULL;
   char *pt = NULL;
   int k1,ts,c1,c2;
   while(!feof(file)) {
     line = readLine(file);
     if(line != NULL) {
       pt = strtok(line,",");
       if(pt != NULL)
        k1 = atoi(pt);
       pt = strtok(NULL,",");
       if(pt != NULL)
        ts = atoi(pt);
       pt = strtok(NULL,",");
       if(pt != NULL)
        c1 = atoi(pt);
       pt = strtok(NULL,",");
       if(pt != NULL)
        c2 = atoi(pt);
       insertRowA(ta,k1,ts,c1,c2);
     }
   }
   DA *columns = newDA(displaySTRING);
   insertDA(columns,newSTRING("K1A"));
   insertDA(columns,newSTRING("TSA"));
   insertDA(columns,newSTRING("C1"));
   insertDA(columns,newSTRING("C2"));
   setColumns(ta,columns);
   setKey(ta,"K1A TSA");
}

static void insertRowA(TABLE *ta, int k1, int ts, int c1, int c2) {
  DA *row = newDA(displayINT);
  insertDA(row,newINT(k1));
  insertDA(row,newINT(ts));
  insertDA(row,newINT(c1));
  insertDA(row,newINT(c2));
  insertRow(ta,row);
}
/** Reads tableB from the given file
  *
  * @param file = the file to read from
  */
static void readTableB(TABLE *tb,FILE *file) {
  char *line = NULL;
  char *pt = NULL;
  int k1,ts,c3,c4,c5;

  while(!feof(file)) {
    line = readLine(file);
    if(line != NULL) {
      pt = strtok(line,",");
      if(pt != NULL)
       k1 = atoi(pt);
      pt = strtok(NULL,",");
      if(pt != NULL)
       ts = atoi(pt);
      pt = strtok(NULL,",");
      if(pt != NULL)
       c3 = atoi(pt);
      pt = strtok(NULL,",");
      if(pt != NULL)
       c4 = atoi(pt);
      pt = strtok(NULL,",");
      if(pt != NULL)
       c5 = atoi(pt);
      insertRowB(tb,k1,ts,c3,c4,c5);
    }
  }
  DA *columns = newDA(displaySTRING);
  insertDA(columns,newSTRING("K1B"));
  insertDA(columns,newSTRING("TSB"));
  insertDA(columns,newSTRING("C3"));
  insertDA(columns,newSTRING("C4"));
  insertDA(columns,newSTRING("C5"));
  setColumns(tb,columns);
  setKey(tb,"K1B TSB");
}

static void insertRowB(TABLE *tb, int k1, int ts, int c3, int c4, int c5) {
  DA *row = newDA(displayINT);
  insertDA(row,newINT(k1));
  insertDA(row,newINT(ts));
  insertDA(row,newINT(c3));
  insertDA(row,newINT(c4));
  insertDA(row,newINT(c5));
  insertRow(tb,row);
}
/** Reads table2 from the given file
  *
  * @param file = the file to read from
  */
static void readTable2(TABLE *t2,FILE *file) {
  char *line = NULL;
  char *pt = NULL;
  int k2,ts2,a1,a2,a3;

  while(!feof(file)) {
    line = readLine(file);
    if(line != NULL) {
      pt = strtok(line,",");
      if(pt != NULL)
       k2 = atoi(pt);
      pt = strtok(NULL,",");
      if(pt != NULL)
       ts2 = atoi(pt);
      pt = strtok(NULL,",");
      if(pt != NULL)
       a1 = atoi(pt);
      pt = strtok(NULL,",");
      if(pt != NULL)
       a2 = atoi(pt);
      pt = strtok(NULL,",");
      if(pt != NULL)
       a3 = atoi(pt);
      insertRow2(t2,k2,ts2,a1,a2,a3);
    }
  }
  DA *columns = newDA(displaySTRING);
  insertDA(columns,newSTRING("K2"));
  insertDA(columns,newSTRING("TS2"));
  insertDA(columns,newSTRING("A1"));
  insertDA(columns,newSTRING("A2"));
  insertDA(columns,newSTRING("A3"));
  setColumns(t2,columns);
  setKey(t2,"K2 TS2");
}

static void insertRow2(TABLE *t2, int k1, int ts, int c3, int c4, int c5) {
  DA *row = newDA(displayINT);
  insertDA(row,newINT(k1));
  insertDA(row,newINT(ts));
  insertDA(row,newINT(c3));
  insertDA(row,newINT(c4));
  insertDA(row,newINT(c5));
  insertRow(t2,row);
}
// Reads queries from queries.txt into a queue
static QUEUE *readQueries() {
  QUEUE *queries = newQUEUE(displayQUERY);
  FILE *file = fopen("queries.txt","r");
  QUERY *q = readQuery(file);
  while(!feof(file)) {
    if(q != NULL) {
      enqueue(queries,q);
    }
    q = readQuery(file);
  }
  fclose(file);
  return queries;
}

/** Reads a query into the QUERY struct.
  *
  * Some of the most convoluted code I've ever written.
  */
static QUERY *readQuery(FILE *fp) {
  QUERY *q = newQUERY();
  STRING *s = NULL;

  char *token = readToken(fp); // GET
  if(token == NULL) return NULL;
  // read columns
  token = readToken(fp);
  while(strcmp(token,"FROM") != 0) {
    if(isLastCharComma(token))
      token = trimLastChar(token);
    addColumn(q,token);
    token = readToken(fp);
  }
  char *aggregate = findAggregateInColumns(getColumns(q));
  if(aggregate != NULL) {
    setAggregate(q,aggregate);
  }
  // next token should be table name
  token = readToken(fp);
  if(isLastCharSemicolon(token)) {
    token = trimLastChar(token);
    setTable(q,token);
    return q;
  }
  setTable(q,token);
  // next token should be either COMBINE or WHERE
  token = readToken(fp);
  if(strcmp(token,"COMBINE") == 0) {
    token = readToken(fp); // table_2
    setTable2(q,token);
    token = readToken(fp); // ON
    s = newSTRING(token);
    QUEUE *on_infix_queue = readInfixQueue(fp,s);
    token = getSTRING(s);
    QUEUE *on_postfix_queue = infixToPostfix(on_infix_queue);
    setOnInfixQueue(q,on_infix_queue);
    setOnPostfixQueue(q,on_postfix_queue);
  }
  if(token != NULL && strcmp(token,"WHERE") == 0) {
    s = newSTRING(token);
    QUEUE *infix_queue = readInfixQueue(fp,s);
    token = getSTRING(s);
    QUEUE *postfix_queue = infixToPostfix(infix_queue);
    setWhereInfixQueue(q,infix_queue);
    setWherePostfixQueue(q,postfix_queue);
  }
  if(token != NULL && strcmp(token,"GROUP") == 0) {
    token = readToken(fp); // BY
    token = readToken(fp); // column
    if(isLastCharSemicolon(token))
      token = trimLastChar(token);
    setGroupBy(q,token);
  }

  return q;
}
// Reads an infix queue from the given file and returns it via s
static QUEUE *readInfixQueue(FILE *fp,STRING *s) {
  // read conditions
  // construct a queue that looks like: [ ( , c1 , = , 5 , ) , ; ] etc.
  char *token = readToken(fp); // where or on clause
  QUEUE *infix_queue = newQUEUE(displaySTRING);
  STRING *c1 = NULL;
  STRING *c2 = NULL;
  STRING *c3 = NULL;
  int size = 0;
  while(strcmp(token,"GROUP") != 0 && strcmp(token,"WHERE") != 0) {
    if(strcmp(token,"AND") == 0 || strcmp(token,"OR") == 0) {
      enqueue(infix_queue,newSTRING(token));
    }
    else {// traverse through token (would be easier if separated by spaces)
      size = strlen(token);
      for(int x = 0; x < size; x++) {
        // either column name, integer, or operator
        c1 = characterToSTRING(token[x]);
        //printf("Character at %d is %s\n",x,getSTRING(c1));
        if(strcmp(getSTRING(c1),";") == 0) {
          enqueue(infix_queue,newSTRING(";"));
          return infix_queue;
        }
        else if(strcmp(getSTRING(c1),"(") == 0 || strcmp(getSTRING(c1),")") == 0)
          enqueue(infix_queue,c1);
        else if(isalpha(*getSTRING(c1))) {                            // column name
          c2 = characterToSTRING(token[x + 1]);  // won't be out of bounds
          if(strcmp(getSTRING(c1),"T") == 0 && x + 2 < size) {// check for TS2
            c3 = characterToSTRING(token[x + 2]);// could be out of bounds
            if(strcmp(getSTRING(c3),"2") == 0) {// TS2
              c1 = newSTRING(strcat(getSTRING(c1),getSTRING(c2)));
              c1 = newSTRING(strcat(getSTRING(c1),getSTRING(c3)));
              enqueue(infix_queue,c1);
              x += 2;
            }
          }
          else {
            c1 = newSTRING(strcat(getSTRING(c1),getSTRING(c2))); // regular column name
            x++;
            enqueue(infix_queue,c1);
          }
        }
        else if(isdigit(*getSTRING(c1))) {                                  // Number
          char *empty = malloc(sizeof(char));
          empty[0] = '\0';
          c3 = newSTRING(empty);
          while(isdigit(*getSTRING(c1))) {
            if(c3 == NULL || c1 == NULL) printf("dern\n");
            c3 = newSTRING(strcat(getSTRING(c3),getSTRING(c1)));
            x++;
            c1 = characterToSTRING(token[x]);
          }
          x--;
          enqueue(infix_queue,c3);
        }
        else {                                             // must be operator
          c2 = characterToSTRING(token[x + 1]);
          if(!isalpha(*getSTRING(c2)) && !isdigit(*getSTRING(c2))) {
            c1 = newSTRING(strcat(getSTRING(c1),getSTRING(c2)));
            x++;
            enqueue(infix_queue,c1);
          }
          else {
            enqueue(infix_queue,c1);
          }
        }
      }
    }
    token = readToken(fp);
  }
  insertSemicolon(infix_queue);
  setSTRING(s,token);
  return infix_queue;
}
// inserts a semicolon at the end of the queue (needed for postfix)
static void insertSemicolon(QUEUE *infix_queue) {
  QUEUE *temp = copyQUEUE(infix_queue);
  STRING *s = NULL;
  int size = sizeQUEUE(infix_queue);
  for(int x = 0; x < size; x++) {
    s = dequeue(temp);
    if(x == size - 1) {
      if(strcmp(getSTRING(s),";") != 0)
        enqueue(infix_queue,newSTRING(";"));
    }
  }
}

static STRING *characterToSTRING(char c) {
  char *s = malloc(2*sizeof(char));
  s[0] = c;//asdkjh
  s[1] = '\0';
  STRING *str = newSTRING(s);
  return str;
}

// processes the queries on the given tables and then prints their result
static void processQueries(QUEUE *queries,TABLE *t1,TABLE *ta,TABLE *tb,TABLE *t2) {
  // construct a table using GET columns FROM table [COMBINE table_2]
      // and WHERE clause if it exists & GROUP BY if it exists
  QUERY *q = NULL;
  TABLE *t = NULL;
  FILE *fp = fopen("awlay.txt","w");
  int num_queries = sizeQUEUE(queries);
  for(int x = 0; x < num_queries; x++) {
    q = dequeue(queries);
    t = newTABLE();

    t = processQuery(q,t1,ta,tb,t2);

    // print output (QUERY followed by TABLE)
    displayQUERY(fp,q);
    fprintf(fp,"\n");
    displayTABLE(fp,t);
    fprintf(fp,"\n\n------------------------------------------------------------------------\n\n");
  }
  fclose(fp);
}

static TABLE *processQuery(QUERY *q,TABLE *t1,TABLE *ta,TABLE *tb,TABLE *t2) {
  TABLE *t = newTABLE();
  // get Query data
  DA *columns = getColumns(q);
  char *table1 = getTable1(q);
  char *table2 = getTable2(q);
  char *group_by = getGroupBy(q);
  // process columns
  if(strcmp(getSTRING(getDA(columns,0)),"*") == 0)
    columns = getAllColumns(table1,table2); // this works!
  else if(group_by == NULL && getAggregate(q) != NULL)
    removeNonAggregate(columns);
  setColumns(t,columns);
  // insert rows
  t = insertRows(t,q,t1,ta,tb,t2);
  return t;
}

static TABLE *insertRows(TABLE *new_table,QUERY *q,TABLE *t1,TABLE *ta,TABLE *tb,TABLE *t2) {
  char *table1 = getTable1(q);
  char *table2 = getTable2(q);
  QUEUE *postfix_queue = getOnPostfixQueue(q);
  // traverse either one whole table or where two tables combine [ON clause]
  if(table2 == NULL) { // traverse table1
    if(strcmp(table1,"T1") == 0)
      new_table = traverseTable(new_table,q,t1);
    else if(strcmp(table1,"T1A") == 0)
      new_table = traverseTable(new_table,q,ta);
    else if(strcmp(table1,"T1B") == 0)
      new_table = traverseTable(new_table,q,tb);
    else if(strcmp(table1,"T2") == 0)
      new_table = traverseTable(new_table,q,t2);
  }
  else { // traverse table1 COMBINE table2
    // options are (T1,T2),(T1A,T2),(T1B,T2),(T2,T1),(T2,T1A),(T2,T1B)
    if(strcmp(table1,"T1") == 0 && strcmp(table2,"T2") == 0) {
      new_table = traverseTable(new_table,q,combineTABLESOn(t1,t2,postfix_queue));
    }
    if(strcmp(table1,"T1A") == 0 && strcmp(table2,"T2") == 0) {
      new_table = traverseTable(new_table,q,combineTABLESOn(ta,t2,postfix_queue));
    }
    if(strcmp(table1,"T1B") == 0 && strcmp(table2,"T2") == 0) {
      new_table = traverseTable(new_table,q,combineTABLESOn(tb,t2,postfix_queue));
    }
    if(strcmp(table1,"T2") == 0 && strcmp(table2,"T1") == 0) {
      new_table = traverseTable(new_table,q,combineTABLESOn(t2,t1,postfix_queue));
    }
    if(strcmp(table1,"T2") == 0 && strcmp(table2,"T1A") == 0) {
      new_table = traverseTable(new_table,q,combineTABLESOn(t2,ta,postfix_queue));
    }
    if(strcmp(table1,"T2") == 0 && strcmp(table2,"T1B") == 0) {
      new_table = traverseTable(new_table,q,combineTABLESOn(t2,tb,postfix_queue));
    }
  }
  return new_table;
}
// Uses a decision tree to determine if the where condition is satisfied
static int satisfiesWhereCondition(QUERY *q,DA *columns,ROW *r) {
  QUEUE *postfix_queue = getWherePostfixQueue(q);
  if(sizeQUEUE(postfix_queue) == 0) return 1;
  DA *values = getROWdata(r);
  return calculatePostfix(postfix_queue,columns,values);
}

static char *substr(char *s,int start_index) {
  STRING *c1 = NULL;
  STRING *c2 = NULL;
  STRING *c3 = NULL;
  int size = strlen(s);
  c1 = characterToSTRING(s[start_index]);
  c2 = characterToSTRING(s[start_index + 1]);
  if(strcmp(getSTRING(c1),"T") == 0 && start_index + 2 < size) {// check for TS2
    c3 = characterToSTRING(s[start_index + 2]);// could be out of bounds
    if(strcmp(getSTRING(c3),"2") == 0) {// TS2
      c1 = newSTRING(strcat(getSTRING(c1),getSTRING(c2)));
      c1 = newSTRING(strcat(getSTRING(c1),getSTRING(c3)));
    }
  }
  else {
    c1 = newSTRING(strcat(getSTRING(c1),getSTRING(c2)));
  }
  return getSTRING(c1);
}

static double calculateAverage(TABLE *t,char *column) {
  double total = 0;
  int value = 0;
  double values = 0;
  for(int x = 0; x < getSize(t); x++) {
    value = getINT(getTableData(t,column,x));
    if(value != -1) {
      values += (double)value;
      total++;
    }
  }
  return values/total;
}

static int calculateMin(TABLE *t,char *column,INTEGER *row_num) {
  int min = getINT(getTableData(t,column,0));
  int value = 0;
  for(int x = 1; x < getSize(t); x++) {
    value = getINT(getTableData(t,column,x));
    if(value != -1 && value < min) {
      min = value;
      setINT(row_num,x);
    }
  }
  return min;
}

static int calculateMax(TABLE *t,char *column,INTEGER *row_num) {
  int max = getINT(getTableData(t,column,0));
  int value = 0;
  for(int x = 1; x < getSize(t); x++) {
    value = getINT(getTableData(t,column,x));
    if(value > max) {
      max = value;
      setINT(row_num,x);
    }
  }
  return max;
}

static int calculateCount(TABLE *t,char *column) {
  int count = 0;
  for(int x = 0; x < getSize(t); x++) {
    if(getINT(getTableData(t,column,x)) != -1)
      count++;
  }
  return count;
}

static TABLE *calculateSingleAggregate(TABLE *t,char *aggregate) {
  DA *single_result = NULL;
  char *type = NULL;
  char *column = NULL;
  INTEGER *row_num = newINT(-1);
  DA *new_columns = getTableColumns(t);
  char *col_name = NULL;
  int result = 0;
  //printf("Aggregate is %s\n",aggregate);
  TABLE *new_table = newTABLE();
  if(aggregate[0] == 'A') {
    type = "AVG";
    column = substr(aggregate,4);
    single_result = newDA(displayREAL);
    insertDA(single_result,newREAL(calculateAverage(t,column)));
  }
  else if(aggregate[0] == 'M' && aggregate[1] == 'I') {
    type = "MIN";
    column = substr(aggregate,4);
    result = calculateMin(t,column,row_num);
  }
  else if(aggregate[0] == 'M') {
    type = "MAX";
    column = substr(aggregate,4);
    result = calculateMax(t,column,row_num);
  }
  else if(aggregate[0] == 'C') {
    type = "COUNT";
    column = substr(aggregate,6);
    result = calculateCount(t,column);
  }
  if(aggregate[0] != 'A') {
    single_result = newDA(displayINT);
    insertDA(single_result,newINT(result));
  }
  // insert new row
  //printf("Inserting new row %d in calculate single aggregate\n",getINT(row_num));
  DA *new_row = newDA(displayINT);
  if(getINT(row_num) == -1) {
    //printf("found a single result\n");
    insertRow(new_table,single_result);
    return new_table;
  }
  else {
    for(int i = 0; i < sizeDA(new_columns); i++) {
      col_name = getSTRING(getDA(new_columns,i));
      if(col_name[0] == 'A' || col_name[0] == 'M')
        column = substr(aggregate,4);
      else if(col_name[0] == 'C' && col_name[1] == 'O')
        column = substr(aggregate,6);
      else
        column = col_name;
      INTEGER *data = getTableData(t,column,getINT(row_num));
      insertDA(new_row,data);
    }
  }
  insertRow(new_table,new_row);
  return new_table;
}

static DA *calculateAggregateWithGroupBy(TABLE *t,char *aggregate,char *group_by,int value) {
  //printf("inside calculateAggregateWithGroupBy\n");
  DA *row = newDA(displayINT);
  int result = 0;
  char *column = NULL;
  int total_nums = 0;
  for(int x = 0; x < getSize(t); x++) {
    // if  values are the same
    if(value == getINT(getTableData(t,group_by,x))) {
      //printf("Values are the same. Value is %d\n",value);
      if(aggregate[0] == 'C') {
        column = substr(aggregate,6);
        result++;
      }
      else if(aggregate[0] == 'M' && aggregate[1] == 'A') {
        column = substr(aggregate,4);
        if(getINT(getTableData(t,column,x)) > result)
          result = getINT(getTableData(t,column,x));
      }
      else if(aggregate[0] == 'M') {
        result = 9999999;
        column = substr(aggregate,4);
        if(getINT(getTableData(t,column,x)) < result)
          result = getINT(getTableData(t,column,x));
      }
      else if(aggregate[0] == 'A') {
        column = substr(aggregate,4);
        result += getINT(getTableData(t,column,x));
      }
      total_nums++;
    }
  }
  if(aggregate[0] == 'A') {
    double real_result = (double)result/(double)total_nums;
    insertDA(row,newINT(value));
    insertDA(row,newREAL(real_result));
    return row;
  }
  insertDA(row,newINT(value));
  insertDA(row,newINT(result));
  return row;
}

static int alreadyCalculated(DA *da,int value) {
  for(int x = 0; x < sizeDA(da); x++) {
    int da_value = getINT(getDA(da,x));
    if(da_value == value)
      return 1;
  }
  return 0;
}

// Calculates the aggregate function if there is one
static TABLE *calculateAggregates(TABLE *t,char *aggregate,char *group_by) {
  // can be AVG, MIN, MAX, COUNT
  // in the form AGGREGATE(COLUMN)
  /*printf("Inside calculate Aggregate.\nTable is:\n");
  displayTABLE(stdout,t);
  printf("\n");*/

  TABLE *new_table = newTABLE();
  if(group_by == NULL) { // Table is single row with total of column
    return calculateSingleAggregate(t,aggregate);
  }
  else {
    DA *already_calculated = newDA(displayINT);
    for(int x = 0; x < getSize(t); x++) {
      int current_value = getINT(getTableData(t,group_by,x));
      if(!alreadyCalculated(already_calculated,current_value)) {
        DA *r = newDA(displayINT);
        r = calculateAggregateWithGroupBy(t,aggregate,group_by,current_value);
        insertDA(already_calculated,newINT(current_value));
        insertRow(new_table,r);
      }
    }
  }
  //setColumns(new_table,getTableColumns(t));

  return new_table;
}
// constructs a new table based on the given table and query
static TABLE *traverseTable(TABLE *new_table,QUERY *q,TABLE *t) {
  int size = getSize(t);
  char *col_name = NULL;
  DA *row = NULL;
  DA *new_columns = getTableColumns(new_table);

  char *aggregate = getAggregate(q);
  if(aggregate != NULL) {
    new_table = traverseTableAggregate(new_table,q,t);
    //printf("New table is:\n");
    //displayTABLE(stdout,new_table);
  }
  else {
    for(int x = 0; x < size; x++) {
      row = newDA(displayINT);
      for(int y = 0; y < sizeDA(new_columns); y++) {
        col_name = getSTRING(getDA(new_columns,y));
        INTEGER *data = getTableData(t,col_name,x);
        insertDA(row,data);
      }
      // need to check on all columns (not just ones in select query)
      if(satisfiesWhereCondition(q,getTableColumns(t),getROWatIndex(t,x))) {
          insertRow(new_table,row);
        }
    }
  }
  return new_table;
}
// constructs a new table based on the given query including the aggregate
static TABLE *traverseTableAggregate(TABLE *new_table,QUERY *q,TABLE *t) {
  // AGGREGATE
  TABLE *aggregate_table = newTABLE();
  setColumns(aggregate_table,getTableColumns(t));
  DA *row = NULL;
  char *col_name = NULL;
  DA *new_columns = getTableColumns(new_table);
  /*printf("inside traverseTableAggregate table is:\n");
  displayTABLE(stdout,t);
  printf("\n");*/
  // construct complete table w/where clause
  for(int x = 0; x < getSize(t); x++) {
    row = newDA(displayINT);
    for(int y = 0; y < sizeDA(getTableColumns(t)); y++) {
      col_name = getSTRING(getDA(getTableColumns(t),y));
      INTEGER *data = getTableData(t,col_name,x);
      ///printf("Inserting %d into row\n",getINT(data));
      insertDA(row,data);
    }
    // need to check on all columns (not just ones in select query)
    if(satisfiesWhereCondition(q,getTableColumns(t),getROWatIndex(t,x))) {
        insertRow(aggregate_table,row);
      }
  }

  /*printf("Aggregate table after where condition is:\n");
  displayTABLE(stdout,aggregate_table);
  printf("\n");*/

  char *aggregate = getAggregate(q);
  char *group_by = getGroupBy(q);
  new_table = calculateAggregates(aggregate_table,aggregate,group_by);
  setColumns(new_table,new_columns);
  //printf("New table is:\n");
  //displayTABLE(stdout,new_table);
  return new_table;
}

static void removeNonAggregate(DA *columns) {
  for(int x = 0; x < sizeDA(columns); x++) {
    if(!isAggregate(getSTRING(getDA(columns,x))))
      removeDAatIndex(columns,x);
  }
}

static char *findAggregateInColumns(DA *columns) {
  char *aggregate = NULL;
  char *column_name = NULL;
  for(int x = 0; x < sizeDA(columns); x++) {
    column_name = getSTRING(getDA(columns,x));
    if(isAggregate(column_name))
        aggregate = column_name;
  }
  return aggregate;
}

static int isAggregate(char *s) {
  if(strstr(s,"AVG") != NULL || strstr(s,"MIN") != NULL ||
     strstr(s,"MAX") != NULL || strstr(s,"COUNT") != NULL)
      return 1;
  else
    return 0;
}

static DA *getAllColumns(char *t1, char *t2) {
  DA *columns = newDA(displaySTRING);
  if(t2 == NULL) { // add columns from just t1
    if(strcmp(t1,"T1") == 0) {
      insertT1Columns(columns);
    }
    else if(strcmp(t1,"T2") == 0) {
      insertT2Columns(columns);
    }
    else if(strcmp(t1,"T1A") == 0) {
      insertT1AColumns(columns);
    }
    else if(strcmp(t1,"T1B") == 0) {
      insertT1BColumns(columns);
    }
  }
  else { // add columns from t1 and t2
    // options are (T1,T2),(T1A,T2),(T1B,T2),(T2,T1),(T2,T1A),(T2,T1B)
    if(strcmp(t1,"T1") == 0 && strcmp(t2,"T2") == 0) {
      insertT1Columns(columns);
      insertT2Columns(columns);
    }
    if(strcmp(t1,"T1A") == 0 && strcmp(t2,"T2") == 0) {
      insertT1AColumns(columns);
      insertT2Columns(columns);
    }
    if(strcmp(t1,"T1B") == 0 && strcmp(t2,"T2") == 0) {
      insertT1BColumns(columns);
      insertT2Columns(columns);
    }
    if(strcmp(t1,"T2") == 0 && strcmp(t2,"T1") == 0) {
      insertT2Columns(columns);
      insertT1Columns(columns);
    }
    if(strcmp(t1,"T2") == 0 && strcmp(t2,"T1A") == 0) {
      insertT2Columns(columns);
      insertT1AColumns(columns);
    }
    if(strcmp(t1,"T2") == 0 && strcmp(t2,"T1B") == 0) {
      insertT2Columns(columns);
      insertT1BColumns(columns);
    }
  }
  return columns;
}

static void insertT1Columns(DA *columns) {
  insertDA(columns,newSTRING("K1A"));
  insertDA(columns,newSTRING("TSA"));
  insertDA(columns,newSTRING("C1"));
  insertDA(columns,newSTRING("C2"));
  insertDA(columns,newSTRING("K1B"));
  insertDA(columns,newSTRING("TSB"));
  insertDA(columns,newSTRING("C3"));
  insertDA(columns,newSTRING("C4"));
  insertDA(columns,newSTRING("C5"));
}

static void insertT2Columns(DA *columns) {
  insertDA(columns,newSTRING("K2"));
  insertDA(columns,newSTRING("TS2"));
  insertDA(columns,newSTRING("A1"));
  insertDA(columns,newSTRING("A2"));
  insertDA(columns,newSTRING("A3"));
}

static void insertT1AColumns(DA *columns) {
  insertDA(columns,newSTRING("K1A"));
  insertDA(columns,newSTRING("TSA"));
  insertDA(columns,newSTRING("C1"));
  insertDA(columns,newSTRING("C2"));
}

static void insertT1BColumns(DA *columns) {
  insertDA(columns,newSTRING("K1B"));
  insertDA(columns,newSTRING("TSB"));
  insertDA(columns,newSTRING("C3"));
  insertDA(columns,newSTRING("C4"));
  insertDA(columns,newSTRING("C5"));
}

static int isLastCharSemicolon(char *s) {
  if(s == NULL) return 0;
  char *last_char = s + strlen(s) - 1;
  if(*last_char == ';')
    return 1;
  else
    return 0;
}

static int isLastCharComma(char *s) {
  if(s == NULL) return 0;
  char *last_char = s + strlen(s) - 1;
  if(*last_char == ',')
    return 1;
  else
    return 0;
}

static char *trimLastChar(char *string) {
  if(string != NULL) {
    int i = 0;
    while(string[i] != '\0') {
      i++;
    }
    string[i-1] = '\0';
  }
  return string;
}
