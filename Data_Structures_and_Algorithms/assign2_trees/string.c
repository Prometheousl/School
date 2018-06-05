/*
 * STRING.c
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
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
comparatorSTRING(void *x, void *y) {
	int i = 0;
	char *str1 = getSTRING(x);
	char *str2 = getSTRING(y);
	/*while(str1[i] != '\0' && str2[i] != '\0') {
		if(str1[i] > str2[i])
			return 1;
		else if(str1[i] == str2[i])
			++i;
		else if(str1[i] < str2[i])
			return -1;
	}
	// If all equal, check string lengths
	int str1_length = strlen(str1);
	int str2_length = strlen(str2);
	if(strlen(str1) > strlen(str2))
		return 1;
	else if(strlen(str1) < strlen(str2))
		return -1;
	else
		return 0;*/
	//printf("Comparing %s with %s\n",str1,str2);
	return strcmp(str1,str2);
	}
