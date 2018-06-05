/*
 * real.c
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "real.h"

struct real
    {
    double value;
    };

REAL *
newREAL(double x) {
    REAL *p = malloc(sizeof(REAL));
    if (p == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    p->value = x;
    return p;
    }

double
getREAL(REAL *v) {
    return v->value;
    }

double
setREAL(REAL *v,double x) {
    double old = v->value;
    v->value = x;
    return old;
    }

void
displayREAL(FILE *fp,void *v) {
    fprintf(fp,"%f",getREAL(v));
    }
	
void
freeREAL(REAL *v) {
	free(v);
	}
