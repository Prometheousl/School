/*
 * cda.c
 *
 *  Created on: Sep 7, 2017
 *      Author: alexl
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cda.h"

struct cda {
	void **data;
	int size;
	int capacity;
	int startIndex;
	int endIndex;
	void (*display)(FILE *,void *);
};
/**
 * The constructor is passed a function that knows how to
 * display the generic value stored in an array slot.
 * That function is stored in a display field of the CDA object.
 *
 * @param d = The display function
 * @return The new CDA
 */
extern CDA *newCDA(void (*display)(FILE *,void *)) {
	CDA *items = malloc(sizeof(CDA));
	assert(items != 0);
	items->size = 0;
	items->capacity = 1;
	items->startIndex = 0;
	items->endIndex = 0;
	items->data = malloc(sizeof(void *));
	assert(items->data != 0);
	items->display = display;
	return items;
}
/**
 * This insert method places the item in the slot just prior to
 * the first item in the filled region.
 * If there is no room for the insertion, the array grows by doubling.
 * It runs in amortized constant time.
 *
 * @param items = The CDA to be inserted to
 * @param value = The value to be inserted
 */
extern void insertCDAfront(CDA *items,void *value) {
	if(items->size == items->capacity) growCDA(items);
	if(items->size == 0) insertCDAback(items,value);
	else {
		items->startIndex = decrementIndex(items,items->startIndex);
		items->data[items->startIndex] = value;
		items->size += 1;
	}
}
/**
 * This insert method places the item in the slot just after the
 * last item in the filled region.
 * If there is no room for the insertion, the array grows by doubling.
 * It runs in amortized constant time.
 *
 * @param items = The CDA to be inserted to
 * @param value = The value to be inserted
 */
extern void insertCDAback(CDA *items,void *value) {
	if(items->size == items->capacity) growCDA(items);
	items->endIndex = incrementIndex(items,items->endIndex);
	items->data[items->endIndex] = value;
	items->size += 1;
}
/**
 * This remove method removes the first item in the filled region.
 * If the ratio of the size to the capacity drops below 25%,
 * the array shrinks by half. The array should never shrink below a
 * capacity of 1.
 * It runs in amortized constant time.
 *
 * @param items = The CDA to be removed from
 * @return The removed item
 */
extern void *removeCDAfront(CDA *items) {
	if(items->size == 0) return 0;
	void *removedItem = items->data[items->startIndex];
	items->startIndex = incrementIndex(items,items->startIndex);
	items->size -= 1;
	if((double)items->size / items->capacity < 0.25) {
		shrinkCDA(items);
	}
	return removedItem;
}
/**
 * This remove method removes the last item in the filled region.
 * If the ratio of the size to the capacity drops below 25%,
 * the array shrinks by half.
 * The array should never shrink below a capacity of 1.
 * It runs in amortized constant time.
 *
 * @param items = The CDA to be removed from
 * @return the removed item
 */
extern void *removeCDAback(CDA *items) {
	if(items->size == 0) return 0;
	void *removedItem = items->data[items->endIndex];
	items->endIndex = decrementIndex(items,items->endIndex);
	items->size -= 1;
	if((double)items->size / items->capacity < 0.25) {
		shrinkCDA(items);
	}
	return removedItem;
}
/**
 * The union method takes two array and moves all the items in the
 * donor array to the recipient array.
 * Suppose the recipient array has the items [3,4,5]
 * in the filled portion and the donor array has the items [1,2]
 * in the filled portion, After the union,
 * the donor array will be empty and recipient array will have the items
 * [3,4,5,1,2] in the filled portion.
 * The union method runs in linear time.
 *
 * @param recipient = The recipient CDA
 * @param donor = The donor CDA
 */
extern void unionCDA(CDA *recipient,CDA *donor) {
	if(donor->size == 0) return;
	int i;
	for(i = 0; i < donor->size; ++i) {
		insertCDAback(recipient,donor->data[(donor->startIndex + i) % donor->capacity]);
	}
	/*for(i = 0; i < donor->size; ++i) {
		removeCDAback(donor);
	}*/
	extractCDA(donor);
}
/**
 * The get method returns the value at the given index,
 * from the perspective of the user. In the user's view,
 * the first item is at index zero, the second item at index 1,
 * and so on. So this routine has to translate between the
 * users view and the internal view
 * (where the first item can be anywhere in the underlying array)
 * It runs in constant time.
 *
 * @param items = The CDA to be "gotten" from
 * @param index = The index of the "gotten" value
 * @return the item at the given index
 */
extern void *getCDA(CDA *items,int index) {
	assert(index < items->size && index >= 0);
	void *item = items->data[(items->startIndex + index) % items->capacity];
	return item;
}
/**
 * The set method updates the value at the given index,
 * from the perspective of the user. If the given index is
 * equal to the size, the value is inserted via the
 * insertCDAback method. If the given index is equal to -1,
 * the value is inserted via the insertCDAfront method.
 * The method returns the replaced value or the null pointer
 * if no value was replaced. It runs in constant time
 * if the array does not need to grow and amortized constant time
 * if it does.
 *
 * @param items = The CDA to be modified
 * @param index = The index of the item to be modified
 * @param value = The value to change the item to
 * @return the old item
 */
extern void *setCDA(CDA *items,int index,void *value) {
	if(index == items->size) {
		insertCDAback(items, value);
		return 0;
	}
	else if(index == -1) {
		insertCDAfront(items,value);
	}
	else if(index < -1 || index > items->size) return 0;
	void *replacedValue = items->data[(items->startIndex + index) % items->capacity];
	items->data[(items->startIndex + index) % items->capacity] = value;
	return replacedValue;
}
/**
 * The extract method returns the underlying C array.
 * The array is shrunk to an exact fit prior to being returned.
 * The DA object gets a new array of capacity one and size zero.
 *
 * @param items = The CDA to be extracted from
 * @return the extracted array
 */
extern void **extractCDA(CDA *items) {
	void **newArray = (void**)malloc(items->size * sizeof(void *));
	assert(newArray != 0);
	int temporary = items->startIndex;
	for(int x = 0; x < items->size; x++) {
		newArray[x] = items->data[temporary];
		temporary = incrementIndex(items,temporary);
	}
	assert(items != 0);
	items->size = 0;
	items->capacity = 1;
	items->startIndex = 0;
	items->endIndex = 0;
	items->data = (void**)realloc(items->data,items->capacity * sizeof(void *));
	return newArray;
}
/**
 * The size method returns the number of items stored in the array.
 *
 * @param items = A CDA
 * @return The CDA's size
 */
extern int sizeCDA(CDA *items) {
	return items->size;
}
/**
 *  The visualize method prints out the filled region,
 *  enclosed in parentheses and separated by commas,
 *  followed by the size of the unfilled region, enclosed in brackets.
 *  If the integers 5, 6, 2, 9, and 1 are stored in the array
 *  (listed from the first item onwards) and the array has
 *  capacity 8, the method would generate this output:
 *  (5,6,2,9,1)(3)
 *  with no preceding or following whitespace.
 *  An empty array with capacity 1 displays as ()(1).
 *
 * @param fp = The file to be written to
 * @param items = The CDA to be visualized
 */
extern void visualizeCDA(FILE *fp,CDA *items) {
	displayCDA(fp,items);
	fprintf(fp,"(%d)",(items->capacity - items->size));
}
/**
 * The display method is similar to the visualize method,
 * except the parenthesized size of the unfilled region is not printed.
 *
 * @param fp = The file to be written to
 * @param items = The CDA to be displayed
 */
extern void displayCDA(FILE *fp,CDA *items) {
	// Need to store start index so its value doesn't get
	// changed in the display loop
	// Loop goes from startIndex to endIndex and displays their values.
	/*int endIndex = items->endIndex;
	fprintf(fp,"(");
	for(int i = 0; i < items->size; ++i) {
		items->display(fp, items->data[endIndex]);
		endIndex = decrementIndex(items,endIndex);
		if(i != items->size - 1) { fprintf(fp,","); }
	}
	fprintf(fp,")");*/
	
	int startIndex = items->startIndex;
	fprintf(fp,"(");
	for(int i = 0; i < items->size; ++i) {
		items->display(fp, items->data[startIndex]);
		startIndex = incrementIndex(items,startIndex);
		if(i != items->size - 1) { fprintf(fp,","); }
	}
	fprintf(fp,")");
}
/**
 * Decrements the given index by one. It loops around to the
 * back of the array if the subtracted index is negative.
 * It does this by adding the capacity to the negative number.
 *
 * @param items = The CDA
 * @param index = The index to be decremented
 * @return the decremented index
 */
extern int decrementIndex(CDA *items,int index) {
	index -= 1;
	if(index < 0) index += items->capacity;
	return index;
}
/**
 * Increments the given index by one. It loops around to the
 * front of the array if the subtracted index is greater than
 * the CDA's capacity.
 * It does this by modding the index with the capacity of the array.
 *
 * @param items = The CDA
 * @param index = The index to be incremented
 * @return the incremented index
 */
extern int incrementIndex(CDA *items,int index) {
	index = (index + 1) % items->capacity;
	return index;
}
/**
 * Grows the CDA by a factor of 2.
 *
 * @param items = The CDA to be grown
 */
extern void growCDA(CDA *items) {
	void **temp = items->data; // Store data in a temporary array
	items->data = (void**)malloc(items->capacity * 2 * sizeof(void *));
	assert(items->data != 0);
	for(int i = 0; i < items->size; ++i) {
		items->data[i] = temp[items->startIndex];
		items->startIndex = incrementIndex(items,items->startIndex);
	}
	free(temp); // Free temporary array
	items->capacity *= 2;
	items->endIndex = items->size - 1;
	items->startIndex = 0;
	// Reset endIndex to be end of array and start index to be front.
}
/**
 * Shrinks the CDA by a factor of 2
 *
 * @param items = The CDA to be shrunk
 */
extern void shrinkCDA(CDA *items) {
	void **temp = items->data;
	items->data = (void**)malloc(items->capacity / 2 * sizeof(void *));
	assert(items->data != 0);
	for(int i = 0; i < items->size; ++i) {
		items->data[i] = temp[items->startIndex];
		items->startIndex = incrementIndex(items,items->startIndex);
	}
	free(temp);
	items->capacity /= 2;
	items->startIndex = 0;
	items->endIndex = items->size - 1;
	// Reset endIndex to be end of array and start index to be front.
}
/**
 * Returns the end index
 *
 * @param items = a CDA
 * @return the end index
 */
extern int endIndexCDA(CDA *items) {
	return items->endIndex;
}
