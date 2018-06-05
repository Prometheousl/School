/*
 * queue.c
 *
 *  Created on: Sep 8, 2017
 *      Author: alexl
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cda.h"
#include "queue.h"

struct cda {
	void **data;
	int size;
	int capacity;
	int startIndex;
	int endIndex;
	void (*display)(FILE *,void *);
};

struct queue {
	CDA *cda;
	void (*display)(FILE *,void *);
};
/**
  * The constructor is passed a function that knows how to display the
  * generic values stored in the queue. That function is
  * stored in a display field of the QUEUE object.
  *
  * @param d = The display function
  * @return the new queue
  */
extern QUEUE *newQUEUE(void (*display)(FILE *,void *)) {
	QUEUE *queue = malloc(sizeof(QUEUE));
	assert(queue != 0);
	queue->cda = newCDA(display);
	queue->display = display;
	return queue;
}
/**
  * The enqueue method runs in constant or amortized constant time.
  * The value to be enqueued is stored in the underlying data structure.
  *
  * @param items = A queue
  * @param value = the value to be inserted into the queue
  */
extern void enqueue(QUEUE *items,void *value) {
	insertCDAback(items->cda,value);
}
/**
  * The dequeue method runs in constant or amortized constant time.
  * The value to be dequeued is removed in the underlying data structure.
  *
  * @param items = a queue
  * @return the value that is taken from the queue
  */
extern void *dequeue(QUEUE *items) {
	return removeCDAfront(items->cda);
}
/**
  * The peek method returns the value ready to come off the queue,
  * but leaves the queue unchanged.
  * It runs in constant time.
  *
  * @param items = The queue to be peeked from
  * @return the peeked value
  */
extern void *peekQUEUE(QUEUE *items) {
	return getCDA(items->cda,0);
}
/**
  * The size method returns the number of items stored in the queue.
  * It runs in amortized constant time.
  *
  * @param items = The queue to take the size from
  * @return the size of the queue
  */
extern int sizeQUEUE(QUEUE *items) {
	return sizeCDA(items->cda);
}
/**
  * This display method prints the items stored in the queue.
  * If the integers 5, 6, 2, 9, and 1 are enqueued in the order given,
  * the method would generate this output:
  * <5,6,2,9,1>
  * with no preceding or following whitespace.
  * An empty queue displays as <>.
  *
  * @param fp = The file to be written to
  * @param items = The queue to display
  */
extern void displayQUEUE(FILE *fp,QUEUE *items) {
	fprintf(fp,"<");
	int i = 0;
	for(i = 0; i < sizeQUEUE(items); ++i) {
		items->display(fp, getCDA(items->cda,i));
		if(i != sizeQUEUE(items) - 1) { fprintf(fp,","); }
	}
	fprintf(fp,">");
}
/**
  * The visualize method simply calls the display method
  * of the data structure upon which the queue is based.
  *
  * @param fp = The file to be written to
  * @param items = The queue to visualize
  */
extern void visualizeQUEUE(FILE *fp,QUEUE *items) {
	displayCDA(fp,items->cda);
}