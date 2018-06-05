#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "gt.h"
#include "bst.h"

typedef struct gtvalue GTVALUE;
// PRIVATE GTVALUE FUNCTIONS
static GTVALUE *newGTVALUE(void *,void (*) (FILE *,void *), int (*) (void *,void *));
static void swapper(BSTNODE *a,BSTNODE *b);

struct gtvalue {
	void *value;
	int count;
	void (*display) (FILE *fp,void *);
	int (*comparator) (void *,void *);
};
/** Creates a new GT value
  *
  * @param value = The value to be stored
  * @param display = The display function
  * @param comparator = The comparator function
  */
static GTVALUE *newGTVALUE(
	void *value,void (*display) (FILE *,void *),
	int (*comparator) (void *,void *)){
		GTVALUE *gtvalue = malloc(sizeof(GTVALUE));
		assert(gtvalue != 0);
		gtvalue->value = value;
		gtvalue->display = display;
		gtvalue->comparator = comparator;
		gtvalue->count = 1;
		return gtvalue;
}
/** Returns a GT's value
  *
  * @param gtv = The GTVALUE to get the value from
  */
static GTVALUE *getGTVALUE(GTVALUE *gtv) {
	return gtv->value;
}
/** Returns a GT's count
  *
  * @param gtv = The GTVALUE to get the count from
  */
static int getGTVALUEcount(GTVALUE *gtv) {
	return gtv->count;
}
/** Increments the GTVALUE's count
  *
  * @param gtv = THe GTVALUE to increment
  */
static void incrementCount(GTVALUE *gtv) {
	gtv->count += 1;
}
/** Decrements the GTVALUE's count
  *
  * @param gtv = THe GTVALUE to decrement
  */
static void decrementCount(GTVALUE *gtv) {
	gtv->count -= 1;
}
/** Displays a GTVALUE
  *
  * @param fp = The file to display to
  * @param v = The GTVALUE to display
  */
static void displayGTVALUE (FILE *fp,void *v) {
	GTVALUE *gtv = (GTVALUE *)v;
	gtv->display(fp,getGTVALUE(gtv));
	if(gtv->count > 1)
		fprintf(fp,"-%d",gtv->count);
}
/** Compares two GTVALUEs
  *
  * @param gtv1 = A GTVALUE
  * @param gtv2 = A GTVALUE
  */
static int compareGTVALUE (void *v1, void *v2) {
	GTVALUE *gtv1 = (GTVALUE*)v1;
	GTVALUE *gtv2 = (GTVALUE*)v2;
	return gtv1->comparator(getGTVALUE(gtv1),getGTVALUE(gtv2));
}
/** A swapper function to be passed to the
  * green tree. Swaps two GTVALUE's\n
  *
  * @param a = A BSTNODE
  * @param b = A BSTNODE
  */
static void swapper(BSTNODE *a,BSTNODE *b) {
	GTVALUE *ga = getBSTNODE(a);
    GTVALUE *gb = getBSTNODE(b);

    /* swap the keys stored in the GT value objects */
    void *vtemp = ga->value;
    ga->value = gb->value;
    gb->value = vtemp;

    /* swap the counts stored in the GT value objects */
    int ctemp = ga->count;
    ga->count = gb->count;
    gb->count = ctemp;
}
struct gt {
	BST *bst;
	int num_words;
	void (*display) (FILE *fp,void *);
	int (*comparator) (void *,void *);
};
/** Increments numWords
  *
  * @param gt = a green tree
  */
static void incrementNumWords(GT *gt) {
	gt->num_words += 1;
}
/** Decrements numWords
  *
  * @param gt = a green tree
  */
static void decrementNumWords(GT *gt) {
	gt->num_words -= 1;
}
/** Constructs a new green tree
  *
  * @param display = a display function
  * @param comparator = a comparator function
  */
extern GT *newGT(
    void (*display)(FILE *,void *),
    int (*comparator)(void *,void *)) {
		GT *gt = malloc(sizeof(GT));
		assert(gt != 0);
		gt->bst = newBST(displayGTVALUE,compareGTVALUE,swapper);
		gt->display = display;
		gt->comparator = comparator;
		gt->num_words = 0;
		return gt;
	}
/** Inserts a value into the green tree
  *
  * @param gt = a green tree
  * @param value = the value to be inserted
  */
extern void insertGT(GT *gt,void *value) {
	//GTVALUE *new_value = newGTVALUE(value,gt->display,gt->comparator);
	GTVALUE *new_value = newGTVALUE(value,gt->display,gt->comparator);
	if(findGT(gt,value) != 0) {
		BSTNODE *node = findBST(gt->bst,new_value);
		incrementCount(getBSTNODE(node));
	}
	else { // not a duplicate so insert normally
		insertBST(gt->bst,new_value);
	}
	incrementNumWords(gt);
}
/** Finds a green tree value.
  * Returns the value's count if it is in the tree
  *
  * @param gt = a green tree
  */
extern int findGT(GT *gt,void *value) {
	GTVALUE *gtv = newGTVALUE(value,gt->display,gt->comparator);
	BSTNODE *bstnode = findBST(gt->bst,gtv);
	 if(bstnode == NULL)
		 return 0;
	 else 
		 return getGTVALUEcount(getBSTNODE(bstnode));
}
/** Deletes a green tree value from the tree
  * If the value's count is greater than one it decrements count
  * Otherwise if count is one it deletes it from the tree
  *
  * @param gt = a green tree
  * @param value = the value to be deleted
  */
extern void deleteGT(GT *gt,void *value) {
	GTVALUE *gtv = newGTVALUE(value,gt->display,gt->comparator);
	BSTNODE *node = findBST(gt->bst,gtv);
	if(node == NULL) {
		printf("Value ");
		gtv->display(stdout,value);
		printf(" not found.\n");
	}
	if(node != NULL) { // If node is in tree
		GTVALUE *node_gtv = getBSTNODE(node);
		decrementCount(node_gtv);
		if(node_gtv->count == 0) {
			// Delete normally
			BSTNODE *leaf = swapToLeafBST(gt->bst,node);
			pruneLeafBST(gt->bst,leaf);
		}
		decrementNumWords(gt);
	}
}
/** Returns the size of the green tree
  *
  * @param gt = a green tree
  */
extern int sizeGT(GT *gt) {
	return sizeBST(gt->bst);
}
/** Returns the number of words in the green tree
  *
  * @param gt = a green tree
  */
extern int wordsGT(GT *gt) {
	return gt->num_words;
}
/** Prints the number of words/phrases,
  * the number of nodes, and the minimum
  * and maximum depths of the tree.
  *
  * @param fp = The file to print to
  * @param gt = a green tree
  */
extern void statisticsGT(FILE *fp,GT *gt) {
	printf("Words/Phrases: %d\n",wordsGT(gt));
	statisticsBST(fp,gt->bst);
}
/** Displays the underlying binary search tree
  * of the green tree (including counts)
  *
  * @param fp = The file to print to
  * @param gt = a green tree
  */
extern void displayGT(FILE *fp,GT *gt) {
	displayBST(fp,gt->bst);
}