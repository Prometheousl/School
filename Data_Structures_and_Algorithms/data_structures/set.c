#include <stdio.h>
#include "da.h"

struct set {
	DA *da;
	void (*display)(FILE *,void *);
};
/** The constructor is passed a function that 
  * knows how to display the generic values stored
  *  in the structure.
  */
extern SET *newSET(void (*display)(FILE *,void *)) {
	SET *set = malloc(sizeof(SET));
	set->display = display;
	return set;
}
/** The makeSET method inserts a wrapped value into the 
  * underlying dynamic array. It returns the index of the 
  * inserted value. 
  * Presumably, the caller of makeSET saves the return 
  * value so that findSET and unionSET can be called 
  * in the future.
  */
extern int makeSET(SET *d,void *value) {
	
}
/** The findSET method returns the index of the 
  * representative of the value stored at the given index. 
  */
extern int findSET(SET *d,int index) {
	
}
/** The union method runs findSET on each of the arguments. 
  * If the arguments have different representatives, 
  * the two disjoint sets are unioned and a value of one 
  * is returned. If they have the same representatives, 
  * then a zero is returned. If the two differing representatives 
  * have the same rank, then the representative with the lower 
  * index becomes the parent of the other.
  */
extern int unionSET(SET *d,int index1,int index2) {
	
}
/** The count method returns current number 
  * of representatives in the set. 
  */
extern int countSET(SET *d) {
	
}
/** The method should print each value in the 
  * underlying dynamic array.
  */
extern int displaySET(FILE *,SET *d) {
	
}