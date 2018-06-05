/*
 * bitmap.h
 *
 * Author: Alex Lay
 * Created on: Feb 5, 2018
 * Course: Operating Systems
 * Desc: This header file contains the function prototypes
 * 		 for a bitmap.
 * Purpose: To define function prototypes for a bitmap
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#ifdef bitmap_64 // If complied with bitmap_64
  #define bitmap_type unsigned long long int
  #define bitmap_wordlength  64
#else	// assumed to be 32 bits
  #define bitmap_type unsigned int
  #define bitmap_wordlength  32
#endif

#include <stdio.h>

typedef struct bitmap BITMAP;

extern BITMAP *newBITMAP(int bits);
extern void freeBITMAP(BITMAP *bm);
extern void setBITMAP(BITMAP *bm, int index);
extern void clearBITMAP(BITMAP *bm, int index);
extern int getBITMAP(BITMAP *bm, int index);
extern void printBITMAP(FILE *fp,BITMAP *bm);

#endif /* BITMAP_H_ */
