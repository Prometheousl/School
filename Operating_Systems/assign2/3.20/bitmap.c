/*
 * bitmap.c
 *
 *  Created on: Feb 15, 2018
 *      Author: alexl
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bitmap.h"

// Using an int to store bits. Could use any
// data type.
 #ifdef bitmap_64 // If compiled with bitmap_64
   #define bitmap_type unsigned long long int
   #define bitmap_shift        6
   #define bitmap_mask        63
   #define bitmap_wordlength  64
   #define bitmap_fmt "%016llx" // Format
 #else	// assumed to be 32 bits
   #define bitmap_type unsigned int
   #define bitmap_shift        5
   #define bitmap_mask        31
   #define bitmap_wordlength  32
   #define bitmap_fmt "%08x" // Format
 #endif

#define bitmap_one        (bitmap_type)1

typedef struct bitmap {
  int bits;	// number of bits in the array
  int words;	// number of words in the array
  bitmap_type *array;
} temp;
/** Allocates a new bitmap object
  *
  * @param bits = The size of the bitmap
  */
extern BITMAP *newBITMAP(int bits) {
  BITMAP *bm = malloc(sizeof(BITMAP));
  assert(bm != 0); // Can't be equal to NULL
  bm->bits = bits;
  bm->words = (bits + bitmap_wordlength - 1) / bitmap_wordlength;
  bm->array = calloc(bm->words, sizeof(bitmap_type));
  return bm;
}
/** Frees the bitmap
  *
  * @param bm = The bitmap to free
  */
extern void freeBITMAP(BITMAP *bm) {
  free(bm->array);
  free(bm);
}
/** Sets a bitmap at index index to 1.
  *
  * @param bm = The bitmap
  * @param index = The index to set
  */
extern void setBITMAP(BITMAP *bm, int index) {
  int word = index >> bitmap_shift;		// n / bitmap_wordlength
  int position = index & bitmap_mask;	// n % bitmap_wordlength
  bm->array[word] |= bitmap_one << position;
}
/** Clears a bitmap at index index to 0
  *
  * @param bm = The bitmap
  * @param index = The index to clear
  */
extern void clearBITMAP(BITMAP *bm, int index) {
  int word = index >> bitmap_shift;
  int position = index & bitmap_mask;
  bm->array[word] &= ~(bitmap_one << position);
}
/** Gets the bitmap value at the specified index
  *
  * @param bm = The bitmap
  * @param index = The index to get
  * @return = Either 1 or 0
  */
extern int getBITMAP(BITMAP *bm, int index) {
  int word = index >> bitmap_shift;         // n / bitmap_wordlength
  int position = index & bitmap_mask;       // n % bitmap_wordlength
  return (bm->array[word] >> position) & 1;
}
/** Prints the bitmap in bit Format
  *
  * @param fp = The file to print to
  * @param bm = The bitmap to print
  */
extern void printBITMAP(FILE *fp, BITMAP *bm) {
  for (int x = 0; x < bm->words; x++) {
    fprintf(fp," " bitmap_fmt, bm->array[x]);
  }
  fprintf(fp,"\n");
}
