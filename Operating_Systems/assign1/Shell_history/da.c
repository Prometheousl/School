/*
 * File: da.c
 *
 * Author: Alex Lay
 * Created on: Sep 6, 2017
 * Course: CS 201 Data Structures and Algorithms
 * Desc: This file implements the functions defined in da.h.
 * Purpose: To be able to implement a fully functional dynamic array.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "da.h"

struct da {
	void **data;
	int size;
	int capacity;
	void (*display)(FILE *,void *);
};

/********************* private methods ******************/

static void growDA(DA *items);
static void shrinkDA(DA *items);
static void reallocateMemory(DA *items);

/********************* public methods *******************/

/**
 * This constructor creates a new DA object and
 * stores the display function passed to it in
 * that object.
 * So, each dynamic array has only one display function.
 *
 * @param display - displays the generic value stored in an array slot
 * @return items - the DA* object
 */
DA * newDA(void (*display)(FILE *,void *)) {
	DA *items = malloc(sizeof(DA));
	assert(items != 0); // Memory allocated cannot be 0.
	items->size = 0;
	items->capacity = 1;
	items->data = malloc(sizeof(void *));
	assert(items->data != 0); // Memory allocated cannot be 0.
	items->display = display;
	return items;
}

/**
 * The insert method places the item in the leftmost unfilled slot.
 * If there is no room for the insertion, the array grows by doubling.
 * It runs in amortized constant time.
 *
 * @param items = the DA* object to be inserted into
 * @param value = the value to be inserted
 */
extern void insertDA(DA *items,void *value) {
	assert(items != 0); // Memory allocated cannot be 0.
	if(items->size == items->capacity) { // Double array capacity
		growDA(items);
	}
	items->data[items->size] = value;
	items->size += 1;
}
/**
 * The remove method removes the rightmost item in the filled region.
 * If the ratio of the size to the capacity drops below 25%,
 * the array shrinks by half.
 * The array should never shrink below a capacity of 1.
 * It runs in amortized constant time.
 *
 * @param items = The DA* object to be removed from
 * @return = the removed item
 */
extern void *removeDA(DA *items) {
	if(items->size == 0) return 0;
	void *removedItem = items->data[items->size - 1];
	items->size -= 1;
	/*if((double)items->size / items->capacity < 0.25) {
		shrinkDA(items);
	}*/
	return removedItem;
}
/**
 * The union method takes two array and moves
 * all the items in the donor array to the recipient array.
 * If the recipient array has the items [3,4,5] and the donor
 * array has the items [1,2], then, after the union, the donor
 * array will be empty and recipient array will have the items
 * [3,4,5,1,2].
 * The union method runs in linear time.
 *
 * @param recipient = The recipient array
 * @param donor = The donor array
 */
extern void unionDA(DA *recipient,DA *donor) {
	if(donor->size == 0) return;
	int i = 0;
	int original_donor_size = donor->size;
	for(i = 0; i < original_donor_size; ++i) {
		void *donatedItem = donor->data[i];
		insertDA(recipient, donatedItem);
	}

	for(i = 0; i < original_donor_size; ++i) {
		removeDA(donor);
	}
}
/**
 * The get method returns the value at the given index.
 * It runs in constant time.
 *
 * @param items = The DA* object
 * @param index = The index to get the value from
 * @return = The value at the given index
 */
extern void *getDA(DA *items,int index) {
	assert(index < items->size && index >= 0);
	void *item = items->data[index];
	return item;
}
/**
 * If the given index is equal to the size, the value is
 * inserted via the insert method.
 * The method returns the replaced value or the null pointer
 * if no value was replaced.
 * It runs in constant time if the array does not need to grow and
 * amortized constant time if it does.
 *
 * @param items = The DA* object to be modified
 * @param index = The index of the value to be modified
 * @param value = The value to modify the given index
 * @return = The old value
 */
extern void *setDA(DA *items,int index,void *value) {
	assert(index >= 0 && index <= items->size);
	if(index == items->size) {
		insertDA(items, value);
		return 0;
	}
	void *replacedValue = items->data[index];
	items->data[index] = value;
	return replacedValue;
}
/**
 * The size method returns the number of items stored in the array.
 *
 * @param items = The DA* object
 * @return = The size of the DA* object
 */
extern int sizeDA(DA *items) {
	return items->size;
}
/**
 * The extract method returns the underlying C array.
 * The array is shrunk to an exact fit prior to being returned.
 * The DA object gets a new array of capacity one and size zero.
 *
 * @param items = The DA* object to be extracted from
 * @return = The extracted array
 */
extern void **extractDA(DA *items) {
	items->capacity = 41;
	reallocateMemory(items);
	void **newArray = items->data;
	items->size = 0;
	items->capacity = 1;
	items->data = malloc(sizeof(void *) * items->capacity);
	assert(items->data != 0);
	return newArray;
}

/**
 * The visualize method prints out the filled region,
 * enclosed in brackets and
 * separated by commas, followed by the size of the unfilled region,
 * enclosed in brackets.
 * If the integers 5, 6, 2, 9, and 1 are stored in the array
 * (listed from index 0 upwards)
 * and the array has capacity 8, the method would generate this output:
 * [5,6,2,9,1][3]
 * with no preceding or following whitespace.
 * An empty array with capacity 1 displays as [][1].
 *
 * @param fp = The file to be written to
 * @param items = The DA* object to be visualized
 */
extern void visualizeDA(FILE *fp, DA *items) {
	displayDA(fp,items);
	fprintf(fp,"[%d]",(items->capacity - items->size));
}
/**
 * The display method is similar to the visualize method,
 * except the bracketed size of the unfilled region is not printed.
 *
 * @param fp = The file to be written to
 * @param items = The DA* object to be displayed
 */
extern void displayDA(FILE *fp,DA *items) {
	fprintf(fp,"[");
	int i = 0;
	for(i = 0; i < items->size; ++i) {
		items->display(fp, items->data[i]);
		if(i != items->size - 1) { fprintf(fp,","); }
	}
	fprintf(fp,"]");
}
/**
 * Reallocates memory for the DA* object based on the DA*
 * object's capacity.
 *
 * @param items = The DA* object to be reallocated
 */
static void reallocateMemory(DA *items) {
	// CANNOT allocate memory for a size of zero!!!
	if(items->capacity != 0) {
		items->data = (void**)realloc(items->data,items->capacity * sizeof(void *));
		assert(items->data != 0);
	}
}
/**
 * Frees the memory in DA
 *
 * @param items = The DA object to be freed
 */
/*extern void freeDA(DA *items) {
	free(items->data);
    free(items);
}*/
/**
 * Shrinks DA's capacity by 2
 *
 * @param items = The DA object to be shrunk
 */
static void shrinkDA(DA *items) {
	void **newArray = (void**)realloc(items->data,items->capacity / 2 * sizeof(void *));
	assert(newArray != 0);
	for(int i = 0; i < items->size; ++i) {
		newArray[i] = getDA(items,i);
	}
	free(items->data);
	items->data = newArray;
	items->capacity /=2;
	// Capacity cannot equal 0.
	if(items->capacity == 0) items->capacity = 1;
	/*items->capacity /= 2;
	if(items->capacity == 0) items->capacity = 1;
	void **newArray = (void **) realloc(items->data,
		(sizeof(void *)*items->capacity));

	assert(newArray != 0);
	items->data = newArray;*/
}
/**
 * Grows DA's capacity by 2
 *
 * @param items = The DA object to be grown
 */
static void growDA(DA *items) {
	/*void **newArray = (void**)malloc(items->capacity * 2 * sizeof(void *)); // Store data in a temporary array
	assert(newArray != 0);
	for(int i = 0; i < items->size; i++) {
		newArray[i] = getDA(items,i);
	}
	//free(items->data); // Free temporary array
	items->data = newArray;
	items->capacity *= 2;*/
	items->capacity = items->capacity*2;
	void **p = (void **) realloc(items->data,
		(sizeof(void *)*items->capacity));

	//Checks if the array's memory was reallocated correctly
	assert(p != 0);
	//Assigns the new pointer to the dynamic array
	items->data = p;
}
