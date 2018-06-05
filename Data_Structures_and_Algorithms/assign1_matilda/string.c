/*
 * STRING.c
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "string.h"

struct STR {
    char *value;
    };

STRING *
newSTRING(char *x) {
    STRING *p = malloc(sizeof(STRING));
    if (p == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    p->value = x;
    return p;
    }

char *
getSTRING(STRING *v) {
    return v->value;
    }

char *
setSTRING(STRING *v,char *x) {
    char *old = v->value;
    v->value = x;
    return old;
    }

void
displaySTRING(FILE *fp,void *v) {
    fprintf(fp,"%s",getSTRING(v));
    }
	
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
	
void
freeSTRING(STRING *s) {
	free(s->value);
	free(s);
}
