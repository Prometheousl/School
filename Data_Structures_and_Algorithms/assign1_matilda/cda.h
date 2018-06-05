/*
 * cda.h
 *
 *  Created on: Sep 7, 2017
 *      Author: alexl
 */

#ifndef CDA_H_
#define CDA_H_

#include <stdio.h>

typedef struct cda CDA;

extern int endIndexCDA(CDA *items);
extern CDA *newCDA(void (*d)(FILE *,void *)); 
extern void insertCDAfront(CDA *items,void *value);
extern void insertCDAback(CDA *items,void *value);
extern void *removeCDAfront(CDA *items);
extern void *removeCDAback(CDA *items);
extern void unionCDA(CDA *recipient,CDA *donor);
extern void *getCDA(CDA *items,int index);
extern void *setCDA(CDA *items,int index,void *value);
extern void **extractCDA(CDA *items);
extern int sizeCDA(CDA *items);
extern void visualizeCDA(FILE *,CDA *items);
extern void displayCDA(FILE *,CDA *items);

//extern void freeCDA(CDA *items);

#endif /* CDA_H_ */
