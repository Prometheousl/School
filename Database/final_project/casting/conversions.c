#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "conversions.h"

/** Converts a STRING struct to a INTEGER struct
  *
  * @param s = The STRING to be converted
  */
extern INTEGER *STRINGtoINTEGER(STRING *s) {
	int value = atof(getSTRING(s));
	INTEGER *temp = newINT(value);
	return temp;
}

extern STRING *INTEGERtoSTRING(INTEGER *i) {
  char *s = malloc(sizeof(char));// = itoa(getINT(i));
	s[0] = '\0';
  sprintf(s,"%d",getINT(i));
  return newSTRING(s);
}
