/*
 *  table.c
 *
 *  Created on: April 7, 2018
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

static int isPrime(TABLE *t,char *d);
static int isPrimary(TABLE *t,char *d);
static int isCandidate(TABLE *t,char *d);
static int isSuperKey(TABLE *t, char *d);
static int is2NF(TABLE *t);
static int is3NF(TABLE *t);

struct functional_dependency {
  char *dependee;
  char *dependent;
};
/** Creates a new DEP object
  *
  * @param d1 = the dependee
  * @param d2 = the dependent
  * @return the created dependency
  */
DEP *newDEP(char *d1, char *d2) {
  DEP *d = malloc(sizeof(DEP));
  if(d == 0) {
    fprintf(stderr,"out of memory\n"); exit(1);
  }
  d->dependee = d1;
  d->dependent = d2;
  return d;
}
/** Returns a dependency
  *
  * @param d = the dependency
  * @return the dependency
  */
static DEP *getDEP(void *d) {
  return (DEP *)d;
}
/** displays a dependency
  *
  * @param fp = the file to display to
  * @param d = the dependency to display
  */
void displayDEP(FILE *fp, void *d) {
  DEP *dep = (DEP *)d;
  fprintf(fp,"(%s)->(%s)",dep->dependee,dep->dependent);
}
/** displays a string
  *
  * @param fp = the file to display to
  * @param s = the string to display
  */
void displaySTRING(FILE *fp, void *s) {
  char *string = (char *)s;
  fprintf(fp,"%s",string);
}

struct table {
    char *primary_key;
    DA *candidate_keys;
    int form;
    char *attributes;
    DA *dependencies;
};

/** Creates a new TABLE object
  *
  * @param pk = the primary key
  * @param cks = the candidate keys
  * @param attr = the attributes
  * @return the created table
  */
TABLE *newTABLE(char *pk,DA *cks,char *attr) {
    TABLE *t = malloc(sizeof(TABLE));
    if (t == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    t->primary_key = pk;
    t->candidate_keys = cks;
    t->form = 1; // initialize to 1NF
    t->attributes = attr;
    t->dependencies = newDA(displayDEP);
    return t;
}
/** Returns the table
  *
  * @param t = the table
  * @return the table
  */
TABLE *getTABLE(TABLE *t) {
  return t;
}
/** Inserts a dependency into the table
  *
  * @param t = the table
  * @param d1 = the dependee
  * @param d2 = the dependent
  */
void insertDEP(TABLE *t,char *d1,char *d2) {
  DEP *new_dep = newDEP(d1,d2);
  insertDA(t->dependencies,new_dep);
}
/** Determines the normal form of the table
  *
  * @param t = The table to check
  * @return the normal form (1,2, or 3)
  */
extern int determineForm(TABLE *t) {
  // form initialized to 1
  if(is2NF(t)) {
    t->form = 2;
    if(is3NF(t))
      t->form = 3;
  }
  return t->form;
}
/** Checks if table is in 3rd Normal Form
  *
  * This occurs when for all functional dependencies X->Y,
  *   X is a superkey OR Y is a prime attribute
  *
  * @param t = The table to check
  * @return 1 if 3NF, 0 otherwise
  */
static int is3NF(TABLE *t) {
  for(int x = 0; x < sizeDA(t->dependencies); x++) {
    if(!isSuperKey(t,getDEP(getDA(t->dependencies,x))->dependee) &&
       !isPrime(t,getDEP(getDA(t->dependencies,x))->dependent))
        return 0;
  }
  return 1;
}
/** Checks if table is in 2nd Normal Form
  *
  * This occurs when "no non-prime attribute is dependent on the
  *    proper subset of any candidate key of the table."
  * In other words, if the dependent & the dependee are prime,
  *    it's not in 2NF.
  *
  * @param t = The table to check
  * @return 1 if 2NF, 0 otherwise
  */
static int is2NF(TABLE *t) {
  // for each dependency
  for(int x = 0; x < sizeDA(t->dependencies); x++) {
    // if prime attribute is dependent on any subset of any candidate key
    // return 0
    if(isPrime(t,getDEP(getDA(t->dependencies,x))->dependent) &&
        isPrime(t,getDEP(getDA(t->dependencies,x))->dependee))
          return 0;
  }
  return 1;
}
/** Checks if the given string is a superkey of the table (basically isPrime)
  *
  * @param t = The table
  * @param d = the given string
  * @return 1 if superkey, 0 otherwise
  */
static int isSuperKey(TABLE *t, char *d) {
  if(strstr(t->primary_key,d) != NULL)
    return 1;
  // check candidate keys
  for(int x = 0; x < sizeDA(t->candidate_keys); x++) {
    if(strstr(getDA(t->candidate_keys,x),d) != NULL)
      return 1;
  }
  return 0;
}
/** Checks if the given string is prime
  *   (either a primary or candidate key)
  *
  * @param t = the table
  * @param d = the string to check
  * @return 1 if prime, 0 otherwise
  */
static int isPrime(TABLE *t,char *d) {
  if(isPrimary(t,d) || isCandidate(t,d))
    return 1;
  else
    return 0;
}
/** Checks if the primary key is in the given string
  *
  * @param t = the table
  * @param d = the string to check
  * @return 1 if primary key in string, 0 otherwise
  */
static int isPrimary(TABLE *t,char *d) {
  if(strstr(t->primary_key,d) != NULL)
    return 1;
  else
    return 0;
}
/** Checks if any candidate keys are in the given string
  *
  * @param t = the table
  * @param d = the string to check
  * @return 1 if candidate key in string, 0 otherwise
  */
static int isCandidate(TABLE *t,char *d) {
  // check candidate keys
  for(int x = 0; x < sizeDA(t->candidate_keys); x++) {
    if(strstr(getDA(t->candidate_keys,x),d) != NULL)
      return 1;
  }
  return 0;
}
/** Helper function to display a table
  *
  * @param fp = the file to display to
  * @param table = the table to display
  */
void displayTable(FILE *fp,void *table) {
  TABLE *t = getTABLE(table);
  fprintf(fp,"Attributes: %s\n",t->attributes);
  fprintf(fp,"Primary key: %s\n",t->primary_key);
  fprintf(fp,"%d candidate key(s):\n",sizeDA(t->candidate_keys));
  displayDA(fp,t->candidate_keys);
  fprintf(fp,"\n%d dependencie(s):\n",sizeDA(t->dependencies));
  displayDA(fp,t->dependencies);
  fprintf(fp,"\n");
}
/** Frees the table
  *
  * @param t = the table to free
  */
void freeTABLE(TABLE *t) {
	free(t);
}
