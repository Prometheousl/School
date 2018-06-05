/**
*	Author: Alex Lay
*	Date: 9/27/2017
*
*	Description: This program implements a binary search tree.
*	It contains methods for inserting, finding and displaying nodes.
*	The display function displays in-order.
*
*	I'm low-key pretty proud of this class. :)
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"

struct node {
	void *value;
	void *key;
	struct node *left, *right;
};

struct BST {
	node *root;
	int size;
	void (*display)(FILE *,void *,void *);
	int (*comparator)(void *,void *);
};

/**
 * Constructor for the node struct.
 *
 * Makes a new node with a key/value pair
 * Static = private...sorta
 */
static node *newNode(void *key, void *value) {
	node *newNode = malloc(sizeof(node)); // Don't forget to malloc when making a new node!
	assert(newNode != 0);
	newNode->key = key;
	newNode->value = value;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

// List of private node functions
static void insertNode(int (*comparator)(void *,void *), node *current, void *key, void *value);
static void *findNode(int (*comparator)(void *,void *), node *current, void *key);
static void inOrder(void (*display)(FILE *,void *,void *),FILE *fp, node *current);
static void freeNodes(node *current);
/**
 * Constructor for BST
 *
 * Makes a new BST. Takes in a display
 * and comparator function with which
 * to display and compare its values/keys.
 */
BST *newBST(void (*display)(FILE *,void *,void *),int (*comparator)(void *,void *)) {
	BST *bst = malloc(sizeof(BST));
	assert(bst != 0); // Memory allocated cannot be 0.
	bst->root = NULL;
	bst->size = 0;
	bst->display = display;
	bst->comparator = comparator;
	return bst;
}
/**
*  The insert method adds a leaf to the tree in the proper location, 
*  based upon the comparator passed to the constructor. It is passed 
*  a generic key value (upon which comparisons are made) 
*  and a generic value that is to be associated with the key.
*
*  @param tree = a pointer to the root of the tree
*  @param key = the key to be inserted
*  @param value = the value to be inserted
*/
extern void insertBST(BST *tree,void *key,void *value) {
	if(tree->root == NULL) { // Node = root
		tree->root = newNode(key,value);
	}
	else { // Recursively insert node into tree
		insertNode(tree->comparator,tree->root,key,value);
	}
	tree->size += 1;
}
/**
*  Recursively inserts a node based on the comparator function
*  passed to it.
*  If comparator returns negative, it sets the node to the left
*  node if it is empty or calls insertNode with the left node passed.
*  Opposite for positive.
*
*  Assumes all keys are unique
*
*  @param comparator = The comparator function to compare values
*  @param current = The current node
*  @param key = The key to be inserted
*  @param value = The value to be inserted
*/
static void insertNode(int (*comparator)(void *,void *), node *current, void *key, void *value) {
	int compare_result = comparator(key, current->key); // key - current->key
	if(compare_result < 0) { // If the comparator returns negative
		if(current->left == NULL)
			current->left = newNode(key,value);
		else
			insertNode(comparator,current->left,key,value);
	}
	else { // If comparator returns positive
		if(current->right == NULL)
			current->right = newNode(key,value);
		else
			insertNode(comparator,current->right,key,value);
	}
}
/**
*  The find method returns the value associated with the given key. 
*  The method returns a null pointer if the key is not in the tree. 
*
*  @param tree = The root of the tree
*  @param key = The key of the node we are searching for
*/
extern void *findBST(BST *tree,void *key) {
	return findNode(tree->comparator,tree->root,key);
}
/**
*  findNode recursively searches the BST for the
*  key passed to it.
*/
static void *findNode(int (*comparator)(void *,void *), node *current, void *key) {
	if(current == NULL)
		return NULL;
	else {
		int compare_result = comparator(key, current->key);
		if(compare_result == 0)
			return current->value;
		else if(compare_result < 0)
			return findNode(comparator,current->left,key);
		else
			return findNode(comparator,current->right,key);
	}
}
/**
*  Returns the size of the BST.
*/
extern int sizeBST(BST *tree) {
	return tree->size;
}
/**
*  The display method performs an in-order traversal of the tree. 
*  At any given node, the method displays the left and right subtrees, 
*  each enclosed with brackets, but only if they exist. A space is placed 
*  between any existing subtree (e.g. after the bracket for a left subtree) 
*  and the node value. An empty tree is displayed as []. 
*  To display a node in the tree, the cached display function is passed the 
*  key and the value stored at the node. No characters are to be printed 
*  after the last closing bracket.
*/
extern void displayBST(FILE *fp,BST *tree) {
	// Inorder is Left subtree then Root then right Subtree
	if(tree->root == NULL) { // If tree is empty
		fprintf(fp,"[");
		fprintf(fp,"]");
	}
	inOrder(tree->display,fp,tree->root);
	//fprintf(fp,"\n");
}
/**
*  Prints the inorder values of the BST.
*/
static void inOrder(void (*display)(FILE *,void *,void *),FILE *fp, node *current) {
	if(current != NULL) {
		fprintf(fp,"[");
		inOrder(display,fp,current->left);
		if(current->left != NULL) fprintf(fp," ");
		display(fp,current->key,current->value);
		if(current->right != NULL) fprintf(fp," ");
		inOrder(display,fp,current->right);
		fprintf(fp,"]");
	}
}

/*extern void freeBST(BST *bst) {
	freeNodes(bst->root);
	free(bst);
}*/
/**
 * Frees nodes in bst
 */
static void freeNodes(node *current) {
	if(current == NULL)
		return;
    freeNodes(current->left); // Free left subtree
    freeNodes(current->right);// Free right subtree
    free(current); // Free node
}