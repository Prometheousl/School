/*
 * STRING_ARR.c
 *
 *  Created on: Feb 5, 2018
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "my_string.h"
#include "str_array.h"
#include "da.h"

struct STR_ARRAY {
    DA *array;
};
/** Creates a new string array.
  * A string array is an array
  *   of character strings
  *
  * @param arr = A 2d char array
  * @return = The new str_array
  */
extern STR_ARRAY
*newSTR_ARRAY(char **arr) {
    STR_ARRAY *p = malloc(sizeof(STR_ARRAY));
    if (p == 0) {
    	fprintf(stderr,"out of memory\n");
      exit(1);
    }
    p->array = newDA(displaySTRING);

    int index = 0;
    while(arr[index]) {
      insertDA(p->array,newSTRING(arr[index]));
      index++;
    }

    return p;
}
/** Returns the underlying DA.
  *
  * @param v = The STR_ARRAY to get from
  * @return = The underlying DA
  */
extern DA *
getSTR_ARRAY(STR_ARRAY *v) {
    return v->array;
}
/** Displays the string array in the format:
  *
  * string string string etc...
  *
  * @param fp = The File to display to
  * @param v = The str_array object
  */
extern void
displaySTR_ARRAY(FILE *fp,void *v) {
  DA *str_arr = getSTR_ARRAY(v);
  for(int index = 0; index < sizeDA(str_arr); index++) {
    if(index != sizeDA(str_arr) - 1)
      fprintf(fp,"%s ",getSTRING(getDA(str_arr,index)));
    else
      fprintf(fp,"%s",getSTRING(getDA(str_arr,index)));
  }
}
