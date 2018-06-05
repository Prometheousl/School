/*
 * int.c
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "int.h"

struct INT {
    int value;
    };

INTEGER *
newINT(int x) {
    INTEGER *p = malloc(sizeof(INTEGER));
    if (p == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    p->value = x;
    return p;
    }

int
getINT(INTEGER *v) {
    return v->value;
    }

int
setINT(INTEGER *v,int x) {
    int old = v->value;
    v->value = x;
    return old;
    }

void
displayINT(FILE *fp,void *v) {
    fprintf(fp,"%d",getINT(v));
    }

int
comparatorINT(void *x, void *y) {
	int result = (*(int*)x - *(int*)y);
	return result;
	}
