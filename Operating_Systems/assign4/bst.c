/**
*	Author: Alex Lay
*	Date: 11/4/2017
*
*	Description: This program implements a binary search tree.
*
*	This module holds the functions that are common to both
*	green binary search trees and red-black trees.
*	Note: your BST module must assume that all inserted values are unique.
*	Note also, that your BST module is different from the last assignment
*	as you will not need the key field and the tree nodes will be made public.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "bst.h"
#include "queue.h"

// List of static functions used
static BSTNODE *newBSTNODE(void *,BSTNODE *);
static void insertNode(
	int (*comparator)(void *,void *),
	BSTNODE *current, BSTNODE *);
static BSTNODE *findNode(
	int (*comparator)(void *,void *),
	BSTNODE *, void *);
static BSTNODE *findPredecessor(BSTNODE *);
static BSTNODE *findSuccessor(BSTNODE *);
static BSTNODE *BSTMinimum(BSTNODE *);
static BSTNODE *BSTMaximum(BSTNODE *);
static void BSTSwap(BSTNODE *,BSTNODE *);
static int minDepth(BSTNODE *);
static int maxDepth(BSTNODE *);
static void displayBreadthFirst(FILE *,int,int,BST *,BSTNODE *);
static int min(int, int);
static int max(int, int);

struct bstnode {
	void *value;
	struct bstnode *parent,*left,*right;
};
/**
 * Constructor for the node struct.
 *
 * Makes a new node with a value
 */
static BSTNODE *newBSTNODE(void *value,BSTNODE *parent) {
	BSTNODE *newNode = malloc(sizeof(BSTNODE));
	assert(newNode != 0);
	newNode->value = value;
	newNode->parent = parent;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}
/** This method should return the value
*	stored in the given search tree node.
*
*	@param n = the bstnode to get value from
*/
extern void *getBSTNODE(BSTNODE *n) {
	return n->value;
}
/** This method sets the given node's value
*
*	@param n = the bstnode to get value from
*/
extern void setBSTNODE(BSTNODE *n,void *value) {
	n->value = value;
}
/** Returns the given node's left node
*
*	@param n = a node
*/
extern BSTNODE *getBSTNODEleft(BSTNODE *n) {
	return n->left;
}
/** Sets the given node's left node
*
*   @param n = a node
*   @param replacement = The replacement node
*/
extern void setBSTNODEleft(BSTNODE *n,BSTNODE *replacement) {
	n->left = replacement;
}
/** Returns the given node's right node
*
*	@param n = a node
*/
extern BSTNODE *getBSTNODEright(BSTNODE *n) {
	return n->right;
}
/** Sets the given node's right node
*
*   @param n = a node
*   @param replacement = The replacement node
*/
extern void setBSTNODEright(BSTNODE *n,BSTNODE *replacement) {
	n->right = replacement;
}
/** Returns the given node's parent
*
*	@param n = a node
*/
extern BSTNODE *getBSTNODEparent(BSTNODE *n) {
	return n->parent;
}
/** Sets the given node's parent node
*
*   @param n = a node
*   @param replacement = The replacement node
*/
extern void setBSTNODEparent(BSTNODE *n,BSTNODE *replacement) {
	n->parent = replacement;
}
/*static void printBSTNODE(BSTNODE *n) {
	printf("Node %s:\n",getSTRING(n->value));
	if(n->parent != NULL)
		printf("Parent: %s\n",getSTRING(n->parent->value));
	if(n->left != NULL)
		printf("Left: %s\n",getSTRING(n->left->value));
	if(n->right != NULL)
		printf("Right: %s\n",getSTRING(n->right->value));
}*/
/** Swaps the two nodes
*
*	@param x = a node
*	@param y = a node
*/
static void BSTSwap(BSTNODE *x,BSTNODE *y) {
	// swap values
	void *vtemp = x->value;
	x->value = y->value;
	y->value = vtemp;
}

struct bst {
	BSTNODE *root;
	int size;
	void (*display)(FILE *,void *);
	int (*comparator)(void *,void *);
	void (*swapper)(BSTNODE *,BSTNODE *);
};
/** The constructor is passed three functions,
*	one that knows how to display the generic
*	value stored in a node, one that can compare
*	two generic values, and one that knows how to
*	swap the two generic values held by BSTNODEs
*	(the swapper function is used by swapToLeafBST).
*
*	@param display = The display function(FILE*,void*)
*	@param comparator = The comparator function(void*,void*)
*	@param swapper = The swapper function(BSTNODE*,BSTNODE*)
*/
extern BST *newBST(
    void (*display)(FILE *,void *),
    int (*comparator)(void *,void *),
    void (*swapper)(BSTNODE *,BSTNODE *)) {
		BST *bst = malloc(sizeof(BST));
		assert(bst != 0); // Memory != 0
		bst->root = NULL;
		bst->size = 0;
		bst->display = display;
		bst->comparator = comparator;
		bst->swapper = swapper;
		return bst;
}
/** This method updates the root pointer of a BST object.
*	It should run in constant time.
*
*	@param t = A BST
*	@param replacement = Replacement root
*/
extern void setBSTroot(BST *t,BSTNODE *replacement) {
	t->root = replacement;
}
/* This method returns the root pointer of a BST object.
*
*  @param t = A BST
*/
extern BSTNODE *getBSTroot(BST *t) {
	return t->root;
}
/**
*  The insert method adds a leaf to the tree in the proper location,
*  based upon the comparator passed to the constructor. It is passed
*  a generic value upon which comparisons are made.
*  Assumes all inserted values are unique.
*
*  @param tree = a pointer to the root of the tree
*  @param value = the value to be inserted
*/
extern BSTNODE *insertBST(BST *t,void *value) {
	BSTNODE *newNode = newBSTNODE(value,NULL);
	if(t->root == NULL) { // Node = root
		setBSTroot(t,newNode);
	}
	else { // Recursively insert node into tree;
		insertNode(t->comparator,t->root,newNode);
		// This updates newNode's parent pointer
		// And updates its parent's left/right pointer
	}
	t->size += 1;
	return newNode;
}
/** Recursively compares nodes to find where to insert it in
*	the tree.
*
*	@param comparator = comparator function
*	@param current = The current node
*	@param value = The value to be inserted
*/
static void insertNode(
	int (*comparator)(void *,void *),
	BSTNODE *current, BSTNODE *newNode) {
		int compare_result = comparator(newNode->value, current->value); // value - current->value
		if(compare_result < 0) { // If the comparator returns negative
			if(current->left == NULL) {
				setBSTNODEparent(newNode,current);
				setBSTNODEleft(current,newNode);
			}
			else
				insertNode(comparator,current->left,newNode);
		}
		else { // If comparator returns positive
			if(current->right == NULL) {
				setBSTNODEparent(newNode,current);
				setBSTNODEright(current,newNode);
			}
			else
				insertNode(comparator,current->right,newNode);
		}
}
/** This method returns the node that holds the searched-for value.
*	If the value is not in the tree, the method should return null.
*	It should run in linear time for a green tree and
*	logarithmic time for a red-black tree.
*
*	@param t = a bst
*	@param value = the value to be searched for
*/
extern BSTNODE *findBST(BST *t,void *value) {
	return findNode(t->comparator,t->root,value);
}
/** Recursively searches the nodes of the bst for the given value
*
*	@param t = A bst
*	@param value = The value to be searched for
*/
static BSTNODE *findNode(int (*comparator)(void *,void *), BSTNODE *current, void *value) {
	if(current == NULL)
		return NULL;
	else {
		int compare_result = comparator(value, current->value);
		if(compare_result == 0)
			return current;
		else if(compare_result < 0)
			return findNode(comparator,current->left,value);
		else
			return findNode(comparator,current->right,value);
	}
}
/** This method is implemented with a call to the swap-to-leaf method followed
*	by a call to the prune-leaf method, returning the pruned node.
*	It should run in linear time for a green tree and
*	logarithmic time for a red-black tree.
*
*	@param t = a bst
*	@param value = The value to be deleted
*/
extern BSTNODE *deleteBST(BST *t,void *value) {
	BSTNODE *node = findBST(t,value); // The node to be deleted
	if(node == NULL) {
		printf("Value ");
		t->display(stdout,getBSTNODE(node));
		printf(" not found.\n");
		return NULL;
	}
	else {
		if(t->swapper == NULL) t->swapper = BSTSwap;
		BSTNODE *leaf = swapToLeafBST(t,node);
		pruneLeafBST(t,leaf);
		return leaf;
	}
}
/** This method takes a node and recursively swaps its value with its
*	predecessor's (or successor's) until a leaf node holds the original
*	value. It calls the BST’s swapper function to actually accomplish the swap,
*	sending the two nodes whose values need to be swapped. If the swapper function
*	is NULL, then the method should just swap the values as normal.
*
*	@param t = a bst
*	@param node = the node to be swapped to a leaf
*/
extern BSTNODE *swapToLeafBST(BST *t,BSTNODE *node) {
	if(node == NULL) {
		return NULL;
	}
	if(node->left == NULL && node->right == NULL) {// It's a leaf!
		return node;
	}
	else { // Recursively swap to leaf based on predecessor or successor
		BSTNODE *node_to_be_swapped = NULL;
		if(node->left != NULL)
			node_to_be_swapped = findPredecessor(node);
		else if(node->right != NULL && node_to_be_swapped == NULL)
			node_to_be_swapped = findSuccessor(node);
		t->swapper(node,node_to_be_swapped);
		node = node_to_be_swapped;
	}
	return swapToLeafBST(t,node);
}
/** Returns the predecessor of the given node
*
*	@param node = a node
*/
static BSTNODE *findPredecessor(BSTNODE *node) {
	if(node == NULL) return NULL;
	else if(node->left != NULL)// return rightmost left child
		return BSTMaximum(node->left);
	// else return the first right child of parent
	BSTNODE *y = node->parent;
	BSTNODE *x = node;
	while(y != NULL && x == y->left) {
		x = y;
		y = y->parent;
	}
	return y;
}
/** Returns the successor of the given node
*
*	@param node = a node
*/
static BSTNODE *findSuccessor(BSTNODE *node) {
	if(node == NULL) return NULL;
	else if(node->right != NULL)
		return BSTMinimum(node->right);
	// else return the first left child of parent
	BSTNODE *y = node->parent;
	BSTNODE *x = node;
	while(y != NULL && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}
/** Returns the leftmost node starting at a node
*
*	@param n = a node
*/
static BSTNODE *BSTMinimum(BSTNODE *n) {
	while(n->left != NULL)
		n = n->left;
	return n;
}
/** Returns the rightmost node starting at a node
*
*	@param n = a node
*/
static BSTNODE *BSTMaximum(BSTNODE *n) {
	while(n->right != NULL) {
		//printf("Going to right child\n");
		n = n->right;
	}
	return n;
}
/** Deletes a leaf from the tree
*
*	@param t = a bst
*	@param leaf = The leaf node to be pruned
*/
extern void pruneLeafBST(BST *t,BSTNODE *leaf) {
	if(leaf == t->root) {
		//printf("Leaf is root\n");
		t->root = NULL;
	}
	else if(leaf->parent->left == leaf) {
		leaf->parent->left = NULL;
		leaf->parent = NULL;
	}
	else if(leaf->parent->right == leaf) {
		leaf->parent->right = NULL;
	}
	t->size -= 1;
}
/** Returns the size of the tree
*
*	@param t = a bst
*/
extern int sizeBST(BST *t) {
	return t->size;
}
/** This method should display the number of nodes
*	in the tree as well as the minimum and maximum
*	heights of the tree. It should run in linear time.
*
*	@param fp = The file to display to
*	@param t = a bst
*/
extern void statisticsBST(FILE *fp,BST *t) {
	fprintf(fp,"Nodes: %d\n",t->size);
	fprintf(fp,"Minimum depth: %d\n",minDepth(t->root));
	fprintf(fp,"Maximum depth: %d\n",maxDepth(t->root));
}
/** Finds the minimum depth of the tree
*
*	@param root = The root of the tree
*/
static int minDepth(BSTNODE *root) {
	if(root == NULL) return 0;
	if(root->left == NULL && root->right == NULL)
		return 1;
	// If left subtree is NULL, recurse right subtree
	if(root->left == NULL)
		return 1;
	// If right subtree is NULL, recurse left subtree
	if(root->right == NULL)
		return 1;

	return min(minDepth(root->left), minDepth(root->right)) + 1;
}
/** Finds the maximum depth of the tree
*
*	@param root = The root of the tree
*/
static int maxDepth(BSTNODE *root) {
	if(root == NULL) return 0;
	if(root->left == NULL && root->right == NULL)
		return 1;
	// If left subtree is NULL, recurse right subtree
	if(root->left == NULL)
		return maxDepth(root->right) + 1;
	// If right subtree is NULL, recurse left subtree
	if(root->right == NULL)
		return maxDepth(root->left) + 1;

	return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}
/** When showing the tree, display the nodes with a breadth-first
*	(left-first) traversal.
*	All nodes at a given level should be on the
*	same line of output. The level number should precede
*	the display of the nodes at that level.
*	Display each node according to the following format:
*	an equals sign if the node is a leaf, followed by
*	the node value, followed by
*	a parenthesized display of the parent's value, followed by
*	a - if the node is the root, a -l if the node is a left child,
*	and a -r otherwise
*
*	This method should run in linear time.
*
* 	@param fp = The file to display to
*	@param t = a bst
*/
extern void displayBST(FILE *fp,BST *t) {
	if(t->root == NULL) fprintf(fp,"EMPTY");
	QUEUE *breadth_first = newQUEUE(t->display);
	BSTNODE *node;
	int level = 0, changed_level = 1;
	enqueue(breadth_first,t->root); // Insert initial node
	enqueue(breadth_first,NULL);	// NULL is used for levels
	while(sizeQUEUE(breadth_first) != 0) { // 	theta(N)
		node = dequeue(breadth_first);
		if(node == NULL) { // Increase level
			level++;
			changed_level = 1;
			enqueue(breadth_first,NULL);
			if(peekQUEUE(breadth_first) == NULL)
				break;
		}
		else {
			// Visit node
			if(changed_level == 0) fprintf(fp," ");
			displayBreadthFirst(fp,level,changed_level,t,node);
			// Add left node
			if(node->left != NULL) {
				enqueue(breadth_first,node->left);
			}
			// Add right node
			if(node->right != NULL) {
				enqueue(breadth_first,node->right);
			}
			changed_level = 0;
		}
	}
	fprintf(fp,"\n");
}
/** Display each node according to the following format:
*	an equals sign if the node is a leaf, followed by
*	the node value, followed by
*	a parenthesized display of the parent's value, followed by
*	a - if the node is the root, a -l if the node is a left child,
*	and a -r otherwise
*
*	@param fp = The file to be printed to
*	@param q = The breadth first queue
*	@param level = The level the node is at
*	@param t = The bst
*	@param node = The node to be displayed
*/
static void displayBreadthFirst(FILE *fp,int level,int changed_level,BST *t,BSTNODE *node) {
	if(changed_level) {
		if(level != 0)
			fprintf(fp,"\n%d: ",level);
		else
			fprintf(fp,"%d: ",level);
	}
	if(node->left == NULL && node->right == NULL)
		fprintf(fp,"=");
	t->display(fp,node->value);
	fprintf(fp,"(");
	if(node == t->root)
		t->display(fp,node->value);
	else t->display(fp,node->parent->value);
	fprintf(fp,")");
	if(node == t->root)
		fprintf(fp,"-");
	else if(node->parent->left == node)
		fprintf(fp,"-l");
	else if(node->parent->right == node)
		fprintf(fp,"-r");
}
static int min(int x, int y) {
	if(x < y)
		return x;
	else return y;
}
static int max(int x, int y) {
	if(x > y)
		return x;
	else return y;
}
