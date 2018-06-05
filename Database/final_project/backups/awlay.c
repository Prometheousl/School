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
#include "table_data/T1A.h"
#include "table_data/T1B.h"
#include "table_data/T2.h"
#include "data_structures/da.h"
#include "data_structures/queue.h"
#include "casting/int.h"
#include "casting/m_string.h"
#include "query.h"
#include "table.h"
#include "decision_tree.h"
// input static functions
static void readTables(TABLEA *ta,TABLEB *,TABLE2 *);
static void readTableA(TABLEA *ta,FILE *file);
static void readTableB(TABLEB *tb,FILE *file);
static void readTable2(TABLE2 *t2,FILE *file);
static QUEUE *readQueries();
static QUERY *readQuery(FILE *fp);
// processing queries static functions
static void processQueries(QUEUE *queries,TABLEA *ta,TABLEB *tb,TABLE2 *t2);
static TABLE *processQuery(QUERY *q,TABLEA *ta,TABLEB *tb,TABLE2 *t2);
static QUEUE *readInfixQueue(FILE *fp);
static void removeNonAggregate(DA *columns);
static int isAggregateInColumns(DA *columns);
static int isAggregate(char *s);
static DA *getAllColumns(char *t1, char *t2);
static DA *getRows(DA *columns,QUERY *q,TABLEA *ta,TABLEB *tb,TABLE2 *t2);
static STRING *characterToSTRING(char c);

static DA *traverseTABLE2CombineTable1B(DA *columns,QUERY *q,TABLEB *tb,TABLE2 *t2);
static void insertT1Columns(DA *columns);
static void insertT2Columns(DA *columns);
static void insertT1AColumns(DA *columns);
static void insertT1BColumns(DA *columns);
// misc static functions
static int isLastCharSemicolon(char *s);
static int isLastCharComma(char *s);
static char *trimLastChar(char *string);
// Don't judge me... I decided not to abstract the 3 tables and this is what it has come to
static DA *traverseTable1(DA *columns,QUERY *q,TABLEA *ta,TABLEB *tb);
static DA *traverseTable1A(DA *columns,QUERY *q,TABLEA *ta);
static DA *traverseTable1B(DA *columns,QUERY *q,TABLEB *tb);
static DA *traverseTable2(DA *columns,QUERY *q,TABLE2 *t2);
static DA *traverseTABLE1CombineTable2(DA *columns,QUERY *q,TABLEA *ta,TABLEB *tb,TABLE2 *t2);
static DA *traverseTABLE1ACombineTable2(DA *columns,QUERY *q,TABLEA *ta,TABLE2 *t2);
static DA *traverseTABLE1BCombineTable2(DA *columns,QUERY *q,TABLEB *tb,TABLE2 *t2);
static DA *traverseTABLE2CombineTable1(DA *columns,QUERY *q,TABLEA *ta,TABLEB *tb,TABLE2 *t2);
static DA *traverseTABLE2CombineTable1A(DA *columns,QUERY *q,TABLEA *ta,TABLE2 *t2);

/** Determines  if a table is in
  *   1NF, 2NF or 3NF for a given set of functional dependencies FDs
  */
int main() {
  TABLEA *ta = newTABLEA();
  TABLEB *tb = newTABLEB();
  TABLE2 *t2 = newTABLE2();

  readTables(ta,tb,t2);
  //printTA(stdout,ta);
  //printTB(stdout,tb);
  //printT2(stdout,t2);

  QUEUE *queries = readQueries();
  displayQUEUE(stdout,queries);
  processQueries(queries,ta,tb,t2);
  return 0;
}
/** Reads the tables from 3 files
  *
  * @return the dynamic array of tables
  */
static void readTables(TABLEA *ta,TABLEB *tb,TABLE2 *t2) {
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
static void readTableA(TABLEA *ta,FILE *file) {
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
}

static void readTableB(TABLEB *tb,FILE *file) {
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
}

static void readTable2(TABLE2 *t2,FILE *file) {
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
}

static QUEUE *readQueries() {
  QUEUE *queries = newQUEUE(displayQUERY);
  FILE *file = fopen("queries.txt","r");
  QUERY *q = readQuery(file);
  while(!feof(file)) {
    if(q != NULL) {
      enqueue(queries,q);
    }
    q = readQuery(file);
    if(q != NULL) {
      printf("Just read query:\n");
      displayQUERY(stdout,q);
    }
  }
  fclose(file);
  return queries;
}

/** Some of the most convoluted code I've ever written.
  *
  */
static QUERY *readQuery(FILE *fp) {
  //printf("In read query!\n");
  QUERY *q = newQUERY();

  char *token = readToken(fp); // GET
  if(token == NULL) return NULL;
  //if(strcmp(token,"GET") != 0)
    //token = readToken(fp);
  // read columns
  token = readToken(fp);
  //printf("Token is %s\n",token);
  while(strcmp(token,"FROM") != 0) {
    if(isLastCharComma(token))
      token = trimLastChar(token);
    addColumn(q,token);
    token = readToken(fp);
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
    token = readToken(fp); // on_clause
    if(isLastCharSemicolon(token)) {
      token = trimLastChar(token);
      //setOnClause(q,token);
      return q;
    }
    //setOnClause(q,token);
    token = readToken(fp); // WHERE
  }
  if(strcmp(token,"WHERE") == 0){
    QUEUE *infix_queue = readInfixQueue(fp);
    printf("\n\nAfter reading infix queue, queue is: \n");
    displayQUEUE(stdout,infix_queue);
    printf("\n\n\n");
    QUEUE *postfix_queue = infixToPostfix(infix_queue);
    printf("Postfix Queue is: \n");
    displayQUEUE(stdout,postfix_queue);
    printf("\n\n");
    setWhereInfixQueue(q,infix_queue);
    setWherePostfixQueue(q,postfix_queue);
  }

  return q;
}

static QUEUE *readInfixQueue(FILE *fp) {
  // read conditions
  /* while character != ; or a space
      read each character/variable into a queue
      convert infix queue to postfix queue via decision_tree
        QUEUE *postfix = infixToPostfix(infix_queue)
      store postfix queue for later solving when variables are defined
        setPostfixQueue(query,postfix_queue)*/
  // construct a queue that looks like: [ ( , c1 , = , 5 , ) , ; ] etc.
  char *token = readToken(fp); // where clause
  QUEUE *infix_queue = newQUEUE(displaySTRING);
  STRING *c1 = NULL;
  STRING *c2 = NULL;
  STRING *c3 = NULL;
  int size = 0;
  while(strcmp(token,"GROUP") != 0) {
    //printf("Token is %s\n",token);
    if(strcmp(token,"AND") == 0 || strcmp(token,"OR") == 0) {
      enqueue(infix_queue,newSTRING(token));
    }
    else {// traverse through token (would be easier if separated by spaces)
      size = strlen(token);
      for(int x = 0; x < size; x++) {
        //printf("Queue is:\n");
        //displayQUEUE(stdout,infix_queue);
        //printf("\n");
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
          //printf("C2 is %s\n",getSTRING(c2));
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
        else if(isdigit(*getSTRING(c1))) {
          //printf("Found a digit!\n");                                     // Number
          char *empty = malloc(sizeof(char));
          empty[0] = '\0';
          c3 = newSTRING(empty);
          while(isdigit(*getSTRING(c1))) {
            //printf("Finding more digits.\n");
            if(c3 == NULL || c1 == NULL) printf("dern\n");
            //else printf("c1 is %s and c3 is  %s\n",getSTRING(c1),getSTRING(c3));
            c3 = newSTRING(strcat(getSTRING(c3),getSTRING(c1)));
            //printf("String after appending it to c3 is %s\n",getSTRING(c3));
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
  return infix_queue;
}

static STRING *characterToSTRING(char c) {
  char *s = malloc(2*sizeof(char));
  s[0] = c;//asdkjh
  s[1] = '\0';
  STRING *str = newSTRING(s);
  //printf("Just allocated a string. String is %s\n",getSTRING(str));
  return str;
}

static void processQueries(QUEUE *queries,TABLEA *ta,TABLEB *tb,TABLE2 *t2) {
  /* construct a table using GET columns FROM table [COMBINE table_2]
       and WHERE clause if it exists & GROUP BY if it exists*/
  QUERY *q = NULL;
  TABLE *t = NULL;
  FILE *fp = fopen("awlay.txt","w");
  printf("Should be processing %d queries.\n",sizeQUEUE(queries));
  int num_queries = sizeQUEUE(queries);
  for(int x = 0; x < num_queries; x++) {
    q = dequeue(queries);
    t = newTABLE();
    printf("Processing query...\n");
    displayQUERY(stdout,q);

    t = processQuery(q,ta,tb,t2);

    //displayQUERY(stdout,q);
    displayQUERY(fp,q);
    displayTABLE(stdout,t);
    displayTABLE(fp,t);
  }
  fclose(fp);
}

static TABLE *processQuery(QUERY *q,TABLEA *ta,TABLEB *tb,TABLE2 *t2) {
  TABLE *t = newTABLE();
  DA *rows = NULL;
  // get data
  DA *columns = getColumns(q);
  char *table1 = getTable1(q);
  char *table2 = getTable2(q);
  char *group_by = getGroupBy(q);
  // process columns
  if(strcmp(getSTRING(getDA(columns,0)),"*") == 0)
    columns = getAllColumns(table1,table2); // this works!
  else if(group_by == NULL && isAggregateInColumns(columns))
    removeNonAggregate(columns);
  setColumns(t,columns);
  // insert rows
  rows = getRows(columns,q,ta,tb,t2);
  setRows(t,rows);
  return t;
}

static DA *getRows(DA *columns,QUERY *q,TABLEA *ta,TABLEB *tb,TABLE2 *t2) {
  printf("Inside getRows!\n");
  DA *rows = newDA(displayROW);
  char *table1 = getTable1(q);
  char *table2 = getTable2(q);
  // traverse either one whole table or where two tables combine
  if(table2 == NULL) { // traverse table1
    if(strcmp(table1,"T1") == 0)
      rows = traverseTable1(columns,q,ta,tb);
    else if(strcmp(table1,"T1A") == 0)
      rows = traverseTable1A(columns,q,ta);
    else if(strcmp(table1,"T1B") == 0)
      rows = traverseTable1B(columns,q,tb);
    else if(strcmp(table1,"T2") == 0)
      rows = traverseTable2(columns,q,t2);
  }
  else { // traverse table1 COMBINE table2
    // options are (T1,T2),(T1A,T2),(T1B,T2),(T2,T1),(T2,T1A),(T2,T1B)
    if(strcmp(table1,"T1") == 0 && strcmp(table2,"T2") == 0) {
      rows = traverseTABLE1CombineTable2(columns,q,ta,tb,t2);
    }
    if(strcmp(table1,"T1A") == 0 && strcmp(table2,"T2") == 0) {
      rows = traverseTABLE1ACombineTable2(columns,q,ta,t2);
    }
    if(strcmp(table1,"T1B") == 0 && strcmp(table2,"T2") == 0) {
      rows = traverseTABLE1BCombineTable2(columns,q,tb,t2);
    }
    if(strcmp(table1,"T2") == 0 && strcmp(table2,"T1") == 0) {
      rows = traverseTABLE2CombineTable1(columns,q,ta,tb,t2);
    }
    if(strcmp(table1,"T2") == 0 && strcmp(table2,"T1A") == 0) {
      rows = traverseTABLE2CombineTable1A(columns,q,ta,t2);
    }
    if(strcmp(table1,"T2") == 0 && strcmp(table2,"T1B") == 0) {
      rows = traverseTABLE2CombineTable1B(columns,q,tb,t2);
    }
  }

  return rows;
}

static int satisfiesWhereCondition(QUERY *q,DA *columns,ROW *r) {
  // use decision tree
  /* Get postfix_queue from query
       QUEUE *postfix_queue = getPostfixQueue(q);
     pass the dynamic array of values to evaluate?
     Then in decision_tree, set variables to equal values
       and then evaluate the postfix queue?
       DA *row = getROW(r);
       return evaluatePostfix(postfix_queue,columns,row);*/
  QUEUE *postfix_queue = getWherePostfixQueue(q);
  /*DA *values = getROWdata(r);
  printf("Column is: ");
  displayDA(stdout,columns);
  printf("\nDA is    : ");
  displayDA(stdout,values);
  printf("\n");
  printf("Postfix Queue is: ");
  displayQUEUE(stdout,postfix_queue);*/
  return calculatePostfix(postfix_queue,columns,values);
}

static int satisfiesOnCondition(QUERY *q,DA *columns,ROW *r) {
  QUEUE *postfix_queue = getOnPostfixQueue(q);
  DA *values = getROWdata(r);
  return calculatePostfix(postfix_queue,columns,values);
}

static INTEGER *getTableData(char *col_name,int index,TABLEA *ta,TABLEB *tb,TABLE2 *t2) {
  //printf("Getting column %s at row index %d\n",col_name,index);
  if(strcmp(col_name,"K1A") == 0 || strcmp(col_name,"TSA") == 0
     || strcmp(col_name,"C1") == 0 || strcmp(col_name,"C2") == 0) {
    if(index > sizeDA(getK1A(ta)) - 1)
      return newINT(-1);
  }
  if(strcmp(col_name,"K1B") == 0 || strcmp(col_name,"TSB") == 0
     || strcmp(col_name,"C3") == 0 || strcmp(col_name,"C4") == 0
     || strcmp(col_name,"C5") == 0) {
    if(index > sizeDA(getK1B(tb)) - 1) {
      //printf("Out of bounds for table 2! Returning -1.\n");
      return newINT(-1);
    }
  }
  if(strcmp(col_name,"K2") == 0 || strcmp(col_name,"TS2") == 0
     || strcmp(col_name,"A1") == 0 || strcmp(col_name,"A2") == 0
     || strcmp(col_name,"A3") == 0) {
    if(index > sizeDA(getK2(t2)) - 1)
      return newINT(-1);
  }
  if(strcmp(col_name,"K1A") == 0)
    return getDA(getK1A(ta),index);
  else if(strcmp(col_name,"TSA") == 0)
    return getDA(getTSA(ta),index);
  else if(strcmp(col_name,"C1") == 0)
    return getDA(getC1(ta),index);
  else if(strcmp(col_name,"C2") == 0)
    return getDA(getC2(ta),index);
  else if(strcmp(col_name,"K1B") == 0)
    return getDA(getK1B(tb),index);
  else if(strcmp(col_name,"TSB") == 0)
    return getDA(getTSB(tb),index);
  else if(strcmp(col_name,"C3") == 0)
    return getDA(getC3(tb),index);
  else if(strcmp(col_name,"C4") == 0)
    return getDA(getC4(tb),index);
  else if(strcmp(col_name,"C5") == 0)
    return getDA(getC5(tb),index);
  else if(strcmp(col_name,"K2") == 0)
    return getDA(getK2(t2),index);
  else if(strcmp(col_name,"TS2") == 0)
    return getDA(getTS2(t2),index);
  else if(strcmp(col_name,"A1") == 0)
    return getDA(getA1(t2),index);
  else if(strcmp(col_name,"A2") == 0)
    return getDA(getA2(t2),index);
  else if(strcmp(col_name,"A3") == 0)
    return getDA(getA3(t2),index);
  return NULL;
}

static DA *traverseTable1(DA *columns,QUERY *q,TABLEA *ta,TABLEB *tb) {
  DA *rows = newDA(displayROW);
  printf("traversing table1.\n");
  int sizeA = getSizeA(ta);
  int sizeB = getSizeB(tb);
  int size = 0;
  if(sizeA >= sizeB)
    size = sizeA;
  else
    size = sizeB;
  char *col_name = NULL;
  DA *row = NULL;
  DA *all_row_values = NULL;
  for(int x = 0; x < size; x++) {
    row = newDA(displayINT);
    all_row_values = newDA(displayINT);
    for(int y = 0; y < sizeDA(columns); y++) {
      col_name = getSTRING(getDA(columns,y));
      INTEGER *data = getTableData(col_name,x,ta,tb,NULL);
      insertDA(row,data);
    }
    row = getAllRowValues("T1",NULL);
    //printf("Inserting row:\n");

    //displayROW(stdout,newROW(row));
    ROW *r = newROW(row);
    // where condition comes after select condition
    // need to check on all columns (not just ones in select query)
    if(satisfiesWhereCondition(q,getAllColumns("T1",NULL),r))
      insertDA(rows,newROW(row));
  }
  return rows;
}

static DA *getAllRowValues(TABLEA *ta, TABLEB *tb, TABLE2 *t2) {
  if(ta == NULL && tb == NULL) { // table2

  }
  else if(tb == NULL && t2 == NULL) { // tablea

  }
  else if(ta == NULL && t2 == NULL) {// tableb

  }
  else if(t2 =)
}

static DA *traverseTable1A(DA *columns,QUERY *q,TABLEA *ta) {

}

static DA *traverseTable1B(DA *columns,QUERY *q,TABLEB *tb) {

}

static DA *traverseTable2(DA *columns,QUERY *q,TABLE2 *t2) {

}

static DA *
traverseTABLE1CombineTable2(DA *columns,QUERY *q,TABLEA *ta,TABLEB *tb,TABLE2 *t2) {

}

static DA *
traverseTABLE1ACombineTable2(DA *columns,QUERY *q,TABLEA *ta,TABLE2 *t2) {

}

static DA *
traverseTABLE1BCombineTable2(DA *columns,QUERY *q,TABLEB *tb,TABLE2 *t2) {

}

static DA *
traverseTABLE2CombineTable1(DA *columns,QUERY *q,TABLEA *ta,TABLEB *tb,TABLE2 *t2) {

}

static DA *
traverseTABLE2CombineTable1A(DA *columns,QUERY *q,TABLEA *ta,TABLE2 *t2) {

}

static DA *
traverseTABLE2CombineTable1B(DA *columns,QUERY *q,TABLEB *tb,TABLE2 *t2) {

}

static void removeNonAggregate(DA *columns) {
  for(int x = 0; x < sizeDA(columns); x++) {
    if(!isAggregate(getDA(columns,x)))
      removeDAatIndex(columns,x);
  }
}

static int isAggregateInColumns(DA *columns) {
  int is_aggregate = 0;
  char *column_name = NULL;
  for(int x = 0; x < sizeDA(columns); x++) {
    column_name = getSTRING(getDA(columns,x));
    if(isAggregate(column_name))
        is_aggregate = 1;
  }
  return is_aggregate;
}

static int isAggregate(char *s) {
  if(strstr(s,"AVG") != NULL || strstr(s,"MIN") != NULL ||
     strstr(s,"MAX") != NULL || strstr(s,"COUNT") != NULL)
      return 1;
  else
    return 0;
}

static DA *getAllColumns(char *t1, char *t2) {
  //printf("Getting all columns!\n");
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
