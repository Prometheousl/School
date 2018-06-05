#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "rbt.h"
#include "bst.h"

// *** Static methods ***
// ****** RBTVALUE ******
typedef struct rbtvalue RBTVALUE;
static RBTVALUE *newRBTVALUE(void *,
			void (*) (FILE *,void *),
			int (*) (void *,void *));
static void *getRBTVALUE(RBTVALUE *);
static char getRBTCOLOR(RBTVALUE *);
static void setRBTCOLOR(RBTVALUE *,char);
static void displayRBTVALUE (FILE *fp,void *v);
static void swapper(BSTNODE *,BSTNODE *);
static void insertionFixUp(BST *,BSTNODE *);
static void deletionFixUp(BST *,BSTNODE *);
// ******   RBT   ******
static BSTNODE *parent(BSTNODE *);
static BSTNODE *grandparent(BSTNODE *);
static BSTNODE *leftChild(BSTNODE *);
static BSTNODE*rightChild(BSTNODE *);
static BSTNODE *uncle(BSTNODE *);
static BSTNODE *sibling(BSTNODE *);
static BSTNODE *nephew(BSTNODE *);
static BSTNODE *niece(BSTNODE *);
static int isLeftChild(BSTNODE *);
static int isRightChild(BSTNODE *);
static char color(BSTNODE *);
static void setColor(BSTNODE *,char);
static int isLinear(BSTNODE *node);
static void leftRotate(BST *bst, BSTNODE *x);
static void rightRotate(BST *bst, BSTNODE *x);
static void rotate(BST *bst,BSTNODE *x);

struct rbtvalue {
	void *value;
	int count;
	char color;
	void (*display) (FILE *,void *);
	int (*comparator) (void *,void *);
};
/** Constructs a new RBTVALUE.
  *
  * @param display = A function to display a RBTVALUE
  * @param comparator = A function to compare two RBTVALUEs
  */
static RBTVALUE *newRBTVALUE(
					void *value, 
					void (*display) (FILE *,void *), 
					int (*comparator) (void *,void *)) {
		RBTVALUE *rbtv = malloc(sizeof(RBTVALUE));
		assert(rbtv != 0);
		rbtv->value = value;
		rbtv->count = 1;
		rbtv->color = 'R';
		rbtv->display = display;
		rbtv->comparator = comparator;
		return rbtv;
}
/** Returns the RBTVALUE's value
  * 
  * @param rbtv = An RBTVALUE
  */
static void *getRBTVALUE(RBTVALUE *rbtv) {
	return rbtv->value;
}
/** Returns the RBTVALUE's color
  *
  * @param rbtv = An RBTVALUE
  */
static char getRBTCOLOR(RBTVALUE *rbtv) {
	return rbtv->color;
}
/** Sets the RBTVALUE's color
  *
  * @param rbtv = An RBTVALUE
  * @param color = 'R' or 'B'
  */
static void setRBTCOLOR(RBTVALUE *rbtv,char color) {
	rbtv->color = color;
}
/** Returns the RBTVALUE's count
  *
  * @param rbtv = An RBTVALUE
  */
static int getRBTVALUEcount(RBTVALUE *rbtv) {
	return rbtv->count;
}
/** Increments the RBTVALUE's count
  *
  * @param rbtv = An RBTVALUE
  */
static void incrementRBTVALUEcount(RBTVALUE *rbtv) {
	rbtv->count += 1;
}
/** Decrements the RBTVALUE's count
  *
  * @param rbtv = An RBTVALUE
  */
static void decrementRBTVALUEcount(RBTVALUE *rbtv) {
	rbtv->count -= 1;
}
/** The display function for an RBTVALUE
  *
  * @param fp = The file to print to
  * @param v = The RBTVALUE to display
  */
static void displayRBTVALUE (FILE *fp,void *v) {
	RBTVALUE *rbtv = (RBTVALUE*)v;
	rbtv->display(fp,getRBTVALUE(rbtv));
	if(rbtv->count > 1)
		fprintf(fp,"-%d",rbtv->count);
	fprintf(fp,"-%c",rbtv->color);
}
/** The comparator function for an RBTVALUe
  *
  * @param v1 = An RBTVALUE
  * @param v2 = An RBTVALUE
  */
static int compareRBTVALUE (void *v1,void *v2) {
	RBTVALUE *rbtv1 = (RBTVALUE*)v1;
	RBTVALUE *rbtv2 = (RBTVALUE*)v2;
	return rbtv1->comparator(getRBTVALUE(rbtv1),getRBTVALUE(rbtv2));
}
/** A generic swapper function for two
  * RBTVALUEs
  */
static void swapper(BSTNODE *a,BSTNODE *b) {
	RBTVALUE *ra = getBSTNODE(a);
    RBTVALUE *rb = getBSTNODE(b);

    /* swap the keys stored in the RBT value objects */
    void *vtemp = ra->value;
    ra->value = rb->value;
    rb->value = vtemp;

    /* swap the counts stored in the RBT value objects */
    int ctemp = ra->count;
    ra->count = rb->count;
    rb->count = ctemp;

    /* note: colors are NOT swapped */
}
struct rbt {
	BST *bst;
	int num_words;
	void (*display) (FILE *fp,void *);
	int (*comparator) (void *,void *);
};
/** Constructor for a RBT
  *
  * @param display = A display function
  * @param comparator = A comparator function
  */
extern RBT *newRBT(
    void (*display)(FILE *,void *),
    int (*comparator)(void *,void *)) {
		RBT *rbt = malloc(sizeof(RBT));
		assert(rbt != 0);
		rbt->bst = newBST(displayRBTVALUE,
			       compareRBTVALUE, swapper);
		rbt->display = display;
		rbt->comparator = comparator;
		rbt->num_words = 0;
		return rbt;
	}
/** Inserts a RBTVALUE into the tree
  *
  * @param rbt = A red black tree
  * @param value = A value to be wrapped in
					a RBTVALUE
  */
extern void insertRBT(RBT *rbt,void *value) {
	RBTVALUE *newRBTV = newRBTVALUE(value,rbt->display,rbt->comparator);
	if(findRBT(rbt,value) != 0) {
		BSTNODE *node = findBST(rbt->bst,newRBTV);
		incrementRBTVALUEcount(getBSTNODE(node));
	}
	else {
		BSTNODE *newNode = insertBST(rbt->bst,newRBTV);
		insertionFixUp(rbt->bst,newNode);
	}
	rbt->num_words += 1;
}
/** Recursively fixes up the colors after an insertionFixUp
  * into the tree
  *
  * @param bst = The underlying bst of the rbt
  * @param node = The current BSTNODE
  */
static void insertionFixUp(BST *bst,BSTNODE *node) {
	while(1) {
		if(getBSTroot(bst) == node) break;
		if(color(parent(node)) == 'B') break;
		if(color(uncle(node)) == 'R') {
			setColor(parent(node),'B');
			setColor(uncle(node),'B');
			setColor(grandparent(node),'R');
			node = grandparent(node);
		}
		else { // uncle must be black
			if(!isLinear(node)) {
				BSTNODE *oldNode = node;
				BSTNODE *oldParent = parent(node);
				rotate(bst,node);
				node = oldParent;
				setBSTNODEparent(node,oldNode);
			}
			setColor(parent(node),'B');
			setColor(grandparent(node),'R');
			// rotate parent to grandparent
			rotate(bst,parent(node));
			break;
		}
	}
	setColor(getBSTroot(bst),'B');
}
// Node helper functions because I'm lazy...
// They all assume the family member exists
static BSTNODE *parent(BSTNODE *node) {
	return getBSTNODEparent(node);
}
static BSTNODE *grandparent(BSTNODE *node) {
	return getBSTNODEparent(getBSTNODEparent(node));
}
static BSTNODE *leftChild(BSTNODE *node) {
	return getBSTNODEleft(node);
}
static BSTNODE *rightChild(BSTNODE *node) {
	return getBSTNODEright(node);
}
static int isLeftChild(BSTNODE *node) {
	if(parent(node) == NULL)
		return 0;
	if(leftChild(parent(node)) == node)
		return 1;
	else return 0;
}
static int isRightChild(BSTNODE *node) {
	if(parent(node) == NULL)
		return 0;
	if(rightChild(parent(node)) == node)
		return 1;
	else return 0;
}
static BSTNODE *uncle(BSTNODE *node) {
    if(isLeftChild(parent(node)))
        return rightChild(grandparent(node));
    else
        return leftChild(grandparent(node));
}
static BSTNODE *sibling(BSTNODE *node) {
	if(isLeftChild(node))
		return rightChild(parent(node));
	else
		return leftChild(parent(node));
}
static BSTNODE *nephew(BSTNODE *node) {
	// Furthest child of sibling
	if(sibling(node) == NULL) return NULL;
	if(isRightChild(node))
		return leftChild(sibling(node));
	else
		return rightChild(sibling(node));
}
static BSTNODE *niece(BSTNODE *node) {
	// Closest child of sibling
	if(sibling(node) == NULL) return NULL;
	if(isRightChild(node))
		return rightChild(sibling(node));
	else
		return leftChild(sibling(node));
}
static char color(BSTNODE *node) {
	if(node == NULL) return 'B';
	else return getRBTCOLOR(getBSTNODE(node));
}
static void setColor(BSTNODE *node,char color) {
	setRBTCOLOR(getBSTNODE(node),color);
}
/** Returns 1 if the node and its parent
  * are either both a left or both a right child
  * Else returns 0.
  */
static int isLinear(BSTNODE *node) {
	if(isLeftChild(node) && !isLeftChild(parent(node)))
		return 0;
	if(isRightChild(node) && !isRightChild(parent(node)))
		return 0;
	return 1;
}
/** Rotates x to its parent
  * Determines if it should be a left or right rotation
  */
static void rotate(BST *bst,BSTNODE *x) {
	if(isLeftChild(x) && !isLeftChild(parent(x)))
		rightRotate(bst,parent(x));
	else if(isRightChild(x) && !isRightChild(parent(x)))
		leftRotate(bst,parent(x));
	else if(leftChild(x) != NULL && isLinear(leftChild(x)))
		rightRotate(bst,parent(x));
	else if(rightChild(x) != NULL && isLinear(rightChild(x)))
		leftRotate(bst,parent(x));
}
static void leftRotate(BST *bst, BSTNODE *x) {
	BSTNODE *y = rightChild(x);
	setBSTNODEright(x,leftChild(y)); // y's left subtree = x's right subtree
	if(leftChild(y) != NULL)
		setBSTNODEparent(leftChild(y),x);
	setBSTNODEparent(y,parent(x));
	if(parent(x) == NULL)
		setBSTroot(bst,y);
	else {
		if(leftChild(parent(x)) == x)
			setBSTNODEleft(parent(x),y);
		else
			setBSTNODEright(parent(x),y);
	}
	setBSTNODEleft(y,x);
	setBSTNODEparent(x,y);
}
static void rightRotate(BST *bst, BSTNODE *x) {
	BSTNODE *y = leftChild(x);
	setBSTNODEleft(x,rightChild(y));
	if(rightChild(y) != NULL)
		setBSTNODEparent(rightChild(y),x);
	setBSTNODEparent(y,parent(x));
	if(parent(x) == NULL)
		setBSTroot(bst,y);
	else {
		if(rightChild(parent(x)) == x)
			setBSTNODEright(parent(x),y);
		else
			setBSTNODEleft(parent(x),y);
	}
	setBSTNODEright(y,x);
	setBSTNODEparent(x,y);
}
/** Finds a RBTVALUE. 
  * Returns its count if it exists.
  */
extern int findRBT(RBT *rbt,void *value) {
	 RBTVALUE *rbtv = newRBTVALUE(value,rbt->display,rbt->comparator);
	 BSTNODE *bstnode = findBST(rbt->bst,rbtv);
	 if(bstnode == NULL)
		 return 0;
	 else 
		 return getRBTVALUEcount(getBSTNODE(bstnode));
}
/** Deletes an RBTVALUE from the tree
  */
extern void deleteRBT(RBT *rbt,void *value) {
	RBTVALUE *rbtv = newRBTVALUE(value,rbt->display,rbt->comparator);
	BSTNODE *node = findBST(rbt->bst,rbtv);
	if(node == NULL) {
		printf("Value ");
		rbtv->display(stdout,value);
		printf(" not found.\n");
	}
	if(node != NULL) {
		RBTVALUE *node_rbtv = getBSTNODE(node);
		decrementRBTVALUEcount(node_rbtv);
		if(getRBTVALUEcount(node_rbtv) == 0) {
			// Delete normally
			node = swapToLeafBST(rbt->bst,node);
			deletionFixUp(rbt->bst,node);
			pruneLeafBST(rbt->bst,node);
		}
		rbt->num_words -= 1;
	}
}
static void deletionFixUp(BST *bst,BSTNODE *node) {
	while(1) {
		if(getBSTroot(bst) == node) break;
		if(color(node) == 'R') break;
		if(color(sibling(node)) == 'R') {
			setColor(parent(node),'R');
			setColor(sibling(node),'B');
			rotate(bst,sibling(node));
			// should have black sibling now
		}
		else if(color(nephew(node)) == 'R') {
			setColor(sibling(node),color(parent(node)));
			setColor(parent(node),'B');
			setColor(nephew(node),'B');
			rotate(bst,sibling(node));
			// subtree and tree is BH balanced
			break;
		}
		else if(color(niece(node)) == 'R') {
			// nephew is black
			setColor(niece(node),'B');
			setColor(sibling(node),'R');
			rotate(bst,niece(node));
			// should have red nephew now
		}
		else { // sibling,niece, and nephew are black
			if(sibling(node) != NULL)
				setColor(sibling(node),'R');
			node = parent(node);
			// Subtree is BH balanced but tree is not
		}
	}
	setColor(node,'B');
}
extern int sizeRBT(RBT *rbt) {
	return sizeBST(rbt->bst);
}
/** Returns the number of words in the tree
*	Includes duplicates
*
*	@param rbt = Red black tree
*/
extern int wordsRBT(RBT *rbt) {
	return rbt->num_words;
}

extern void statisticsRBT(FILE *fp,RBT *rbt) {
	printf("Words/Phrases: %d\n",wordsRBT(rbt));
	statisticsBST(fp,rbt->bst);
}
extern void displayRBT(FILE *fp,RBT *rbt) {
	displayBST(fp,rbt->bst);
}