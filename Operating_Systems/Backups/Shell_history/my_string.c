/*
 * STRING.c
 *
 *  Created on: Sep 6, 2017
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "my_string.h"

struct STR {
    char *value;
    };

/** Creates a new STRING object
  *
  * @param x = The string to cast to STRING
  * @return = the STRING object
  */
STRING *
newSTRING(char *x) {
    STRING *p = malloc(sizeof(STRING));
    if (p == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    p->value = x;
    return p;
    }
/** Returns the underlying string
  *
  * @param v = the STRING object
  * @return = the underlying string
  */
char *
getSTRING(STRING *v) {
    return v->value;
    }
/** Sets the STRING object to string
  *
  * @param v = the STRING object
  * @param x = the string to set the
  *   STRING object to
  * @return = the string
  */
char *
setSTRING(STRING *v,char *x) {
    char *old = v->value;
    v->value = x;
    return old;
    }
/** Displays the STRING object
  *
  * @param fp = the file to display to
  * @param v = The STRING object
  */
void
displaySTRING(FILE *fp,void *v) {
    fprintf(fp,"%s",getSTRING(v));
    }
/** Compares two STRINGS
  *
  * @param x,y = STRING objects
  * @return = Result of comparison
  */
int
compareSTRING(void *x, void *y) {
	int i = 0;
	char *str1 = getSTRING(x);
	char *str2 = getSTRING(y);
	while(str1[i] != '\0' && str2[i] != '\0') {
		if(str1[i] > str2[i])
			return 1;
		else if(str1[i] == str2[i])
			++i;
		else if(str1[i] < str2[i])
			return -1;
	}
	// If all equal exit the while loop and return 0
	return 0;
	}
/** Frees the string object
  *
  * @param s =- the SRING object to free
  */
void
freeSTRING(STRING *s) {
	free(s->value);
	free(s);
}
