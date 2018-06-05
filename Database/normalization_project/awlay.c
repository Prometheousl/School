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
#include "scanner.h"
#include "da.h"
#include "table.h"

static DA *readTables();
static TABLE *readTable(FILE *file);
static char *trimSemicolon(char *string);
static DA *readCandidates(FILE *file);
static void insertDependencies(TABLE *t,FILE *file);
static void displayString(FILE *fp,void *string);
static char *getString(void *string);

/** Determines  if a table is in
  *   1NF, 2NF or 3NF for a given set of functional dependencies FDs
  */
int main() {
  // read in each table
  DA *tables = readTables();
  //displayDA(stdout,tables);
  // for each table,
  //  determine degree of normalization
  //  output degree of normalization to stdout & a file
  int degree = 0;
  FILE *file = fopen("HW7awlay.txt","w");
  for(int x = 0; x < sizeDA(tables); x++) {
    degree = determineForm(getDA(tables,x));
    printf("%dNF\n",degree);
    fprintf(file,"%dNF\n",degree);
  }
  return 0;
}
/** Reads the tables from a file called "HW7in.txt"
  *   into a dynamic array
  *
  * @return the dynamic array of tables
  */
static DA *readTables() {
  DA *tables = newDA(displayTable);

  FILE *file = fopen("HW7in.txt","r");
  TABLE *t = readTable(file);
  while(!feof(file)) {
    if(t != NULL) {
      insertDA(tables,t);
    }
    t = readTable(file);
  }
  int unsuccessful_close = fclose(file);
  if(unsuccessful_close)
    printf("Couldn't close file!\n");

  return tables;
}
/** Reads a table from the given file &
  *   creates a new table object from it
  *
  * @param file = the file to read from
  * @return the table created from the file
  */
static TABLE *readTable(FILE *file) {
  // read attributes, primary key, and candidate key(s)
  char *attr = readToken(file);
  char *pk = readToken(file);
  DA *candidate_keys = readCandidates(file);
  // create table
  TABLE *t = newTABLE(pk,candidate_keys,attr);
  // insert dependencies into table
  insertDependencies(t,file);
  return t;
}
/** Reads the candidate keys from the file
  *
  * There can be multiple candidate keys so it
  *   continuously reads until it comes upon a
  *   dependency.
  *
  * @param file = the file to read from
  * @return the dynamic array of candidate keys
  */
static DA *readCandidates(FILE *file) {
  DA *candidate_keys = newDA(displayString);
  char *ck;
  // if next token is not a dependency
  while(!dependencyPending(file)) {
    ck = readToken(file);
    //printf("CK is %s\n",ck);
    if(ck == NULL)
      break;
    else {// it's a candidate key
      //printf("Inserting %s into candidate keys\n",ck);
      if(strchr(ck,';') == NULL)
        insertDA(candidate_keys,ck);
      else
        insertDA(candidate_keys,trimSemicolon(ck));
    }
  }
  return candidate_keys;
}
/** inserts the dependencies from the file into
  *   the newly created table
  *
  * they are in the form (AB)->(D);
  *
  * @param t = the table to insert into
  * @param file = the file to read from
  */
static void insertDependencies(TABLE *t,FILE *file) {
  char *dependency = readLine(file);
  if(dependency == NULL) return;
  char *d1 = NULL;
  char *d2 = NULL;
  while(strcmp(dependency,"x;") != 0) {
    //printf("Dependency is %s\n",dependency);
    //printf("Result of strcmp is %d\n",strcmp("x;\r",dependency));
    d1 = trimSemicolon(strtok(dependency,"("));
    d2 = trimSemicolon(trimSemicolon(strtok(NULL,"(")));
    //printf("D2 SHOULD BE %s\n",d2);
    //printf("Inserting dependency (%s)->(%s)\n",d1,d2);
    insertDEP(t,d1,d2);
    dependency = readLine(file);
  }
}
/** Casts a void * to a string
  *
  * @param string = the void * to cast
  * @return a string
  */
static char *getString(void *string) {
  return (char *)string;
}
/** displays a string
  *
  * @param fp = the file to display to
  * @param string = the string to display
  */
static void displayString(FILE *fp,void *string) {
  char *s = (char*)string;
  fprintf(fp,s);
}
/** Trims the last character from a string
  *
  * @param string = the string to trim
  * @return the trimmed string
  */
static char *trimSemicolon(char *string) {
  if(string != NULL) {
    int i = 0;
    while(string[i] != '\0') {
      i++;
    }
    string[i-1] = '\0';
  }
  return string;
}
