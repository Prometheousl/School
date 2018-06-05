#ifndef __BST_INCLUDED__
#define __BST_INCLUDED__

#include <stdio.h>

typedef struct node node;
typedef struct BST BST;

// Node functions
/*static node *newNode(void *key, void *value);
static void insertNode(int (*comparator)(void *,void *), node *current, void *key, void *value);
static void *findNode(int (*comparator)(void *,void *), node *current, void *key);
static void inOrder(void (*display)(FILE *,void *),FILE *fp, node *current);*/

// BST functions
extern BST *newBST(void (*display)(FILE *,void *,void *),int (*comparator)(void *,void *));
extern void insertBST(BST *tree,void *key,void *value);
extern void *findBST(BST *tree,void *key);
extern int sizeBST(BST *tree);
extern void displayBST(FILE *fp,BST *tree);

//extern void freeBST(BST *bst);
#endif