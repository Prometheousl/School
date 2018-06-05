/*
 * File: da.h
 *
 * Author: Alex Lay
 * Created on: Sep 6, 2017
 * Course: CS 201 Data Structures and Algorithms
 * Desc: This header file contains the function prototypes
 * 		 for a dynamic array.
 * Purpose: To define function prototypes for a dynamic array.
 */

#ifndef DA_H_
#define DA_H_

#include <stdio.h>

typedef struct da DA;

/**
 * The constructor is passed a function that knows how to display the
 * generic value stored in an array slot. That function is stored in a
 * display field of the DA object.
 *
 * @param d = a display function
 * @return = the new DA* object
 */
extern DA *newDA(void (*display)(FILE *,void *));
/**
 * The insert method places the item in the leftmost unfilled slot.
 * If there is no room for the insertion, the array grows by doubling.
 * It runs in amortized constant time.
 *
 * @param items = the DA* object to be inserted into
 * @param value = the value to be inserted
 */
extern void insertDA(DA *items,void *value);
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
extern void *removeDA(DA *items);
/**
 * The union method takes two array and moves all the items
 * in the donor array
 * to the recipient array. If the recipient array has the items [3,4,5]
 * and the donor array has the items [1,2], then, after the union,
 * the donor array will be empty and recipient array will have
 * the items [3,4,5,1,2].
 * The union method runs in linear time.
 *
 * @param recipient = The recipient array
 * @param donor = The donor array
 */
extern void unionDA(DA *recipient,DA *donor);
/**
 * The get method returns the value at the given index.
 * It runs in constant time.
 *
 * @param items = The DA* object
 * @param index = The index to get the value from
 * @return = The value at the given index
 */
extern void *getDA(DA *items,int index);
/**
 * If the given index is equal to the size, the value is inserted
 * via the insert method.
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
extern void *setDA(DA *items,int index,void *value);
/**
 * The extract method returns the underlying C array.
 * The array is shrunk to an exact fit prior to being returned.
 * The DA object gets a new array of capacity one and size zero.
 *
 * @param items = The DA* object to be extracted from
 * @return = The extracted array
 */
extern void **extractDA(DA *items);
/**
 * The size method returns the number of items stored in the array.
 *
 * @param items = The DA* object
 * @return = The size of the DA* object
 */
extern int sizeDA(DA *items);
/**
 * The visualize method prints out the filled region,
 * enclosed in brackets and separated by commas, followed by the size
 * of the unfilled region, enclosed in brackets.
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
extern void visualizeDA(FILE *fp,DA *items);
/**
 * The display method is similar to the visualize method,
 * except the bracketed size of the unfilled region is not printed.
 *
 * @param fp = The file to be written to
 * @param items = The DA* object to be displayed
 */
extern void displayDA(FILE *fp,DA *items);
/**
 * Reallocates memory for the DA* object based on the
 * DA* object's capacity.
 *
 * @param items = The DA* object to be reallocated
 */
extern void reallocateMemory(DA *items);

#endif /* DA_H_ */
