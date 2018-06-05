/*
 * buffer.h
 *
 * Author: Alex Lay
 * Created on: Feb 25, 2018
 * Course: Operating Systems
 * Desc: This header file contains the basic
 *        structure of the buffer used in Ch. 5 project 3
 */

#ifndef BUFFER_H_
#define BUFFER_H_

typedef int buffer_item;
#define BUFFER_SIZE 5

extern void initializeBuffer();
extern int insertItem(buffer_item item);
extern int removeItem(buffer_item *item);
extern void printBuffer();

#endif /* BUFFER_H_ */
