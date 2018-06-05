/*
 * test-all.c
 *
 *	Tests all of the classes in the first project
 *
 *  Created on: Sep 9, 2017
 *  Author: alexl
 */

/*#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "int.h"
#include "da.h"
#include "cda.h"
#include "stack.h"
#include "queue.h"

void fillDA(DA *x) {
	int i = 0;
	for(i = 1; i < 10; ++i) {
		insertDA(x,newINT(i));
	}
}

int
main(int argc,char **argv) {
    if (argc != 1) {
    	fprintf(stderr,"usage: %s\n",argv[0]); exit(1);
    }
    int i = 0;
    printf("da tester, version 1\n");
    printf("Testing newDA & insertDA***********\n");
    DA *x = newDA(displayINT);
    visualizeDA(stdout,x);
    for(i = 1; i < 10; ++i) {
    	printf("\n");
    	insertDA(x,newINT(i));
    	visualizeDA(stdout,x);
    }

    printf("Testing removeDA***********\n");
    for(i = 1; i < 10; ++i) {
        printf("\n");
        removeDA(x);
        visualizeDA(stdout,x);
    }

    fillDA(x);

    printf("Testing Union*********\n");
    DA *y = newDA(displayINT);
    for(i = 1; i < 5; ++i) {
    	printf("\n");
    	insertDA(y,newINT(i));
    	visualizeDA(stdout,y);
    }
    printf("\nDonating x values into y.\nx: ");
    visualizeDA(stdout,x);
    printf("y: ");
    visualizeDA(stdout,y);
    unionDA(y,x);
    printf("\nAfter donation\nx: ");
    visualizeDA(stdout,x);
    printf("y: ");
    visualizeDA(stdout,y);

    fillDA(x);

    printf("Testing get and setDA******\n");
    void *replacedValue = setDA(x,8,newINT(90));
    //void *newValue = getDA(x,4);
    printf("Replaced %d ", *(int*)replacedValue);
    printf("with %d",*(int*)getDA(x,8));
    visualizeDA(stdout,x);

    printf("Testing sizeDA********\n");
    int size = sizeDA(x);
    printf("Size of DA is %d.",size);

    printf("Testing extractDA*********\n");
    extractDA(x);

    printf("\nOld array: ");
    visualizeDA(stdout,x);

    STACK *s = newSTACK(displayINT);
    displaySTACK(stdout,s);
    printf("\nPushing to stack...");
    for(int i = 1; i < 10; ++i) {
       printf("\n");
        push(s,newINT(i));
        displaySTACK(stdout,s);
    }
    printf("\nPopping top value...");
    void *poppedItem = pop(s);
    printf("Popped item is %d.",*(int *)poppedItem);

    printf("\nPeeking at top value...");
    void *peekedItem = peekSTACK(s);
    printf("\nPeeked item is %d.",*(int *)peekedItem);

    printf("\nStack size is %d.",sizeSTACK(s));

    printf("Calling visualizeSTACK...\n");
    visualizeSTACK(stdout,s);

    CDA *c = newCDA(displayINT);
        displayCDA(stdout,c);
        printf("\nInserting to front of CDA...");
        for(int i = 1; i < 11; ++i) {
        	printf("\n");
        	insertCDAfront(c,newINT(i));
        	visualizeCDA(stdout,c);
        	printf("\nSize is %d.",sizeCDA(c));
        }
        printf("\nRemoving from front of CDA...");
        for(int i = 1; i < 11; ++i) {
            printf("\n");
            void *removedValue = removeCDAfront(c);
            visualizeCDA(stdout,c);
            printf("\nSize is %d.",sizeCDA(c));
            printf("\nRemoved value is %d.",*(int*)removedValue);
        }

        printf("\nInserting to back of CDA...");
        for(int i = 1; i < 10; ++i) {
            printf("\n");
            insertCDAback(c,newINT(i));
            visualizeCDA(stdout,c);
            printf("\nSize is %d.",sizeCDA(c));
        }

        printf("\nRemoving from back of CDA...");
        for(int i = 1; i < 10; ++i) {
            printf("\n");
            void *removedValue = removeCDAback(c);
            visualizeCDA(stdout,c);
            printf("\nSize is %d.",sizeCDA(c));
            printf("\nRemoved value is %d.",*(int*)removedValue);
        }
        printf("\nTesting union\n");
        insertCDAback(c,newINT(90));
        visualizeCDA(stdout,c);
        insertCDAback(c,newINT(7));
        visualizeCDA(stdout,c);
        insertCDAback(c,newINT(23));
        visualizeCDA(stdout,c);

        CDA *d = newCDA(displayINT);
        insertCDAfront(d,newINT(2));
        insertCDAfront(d,newINT(5));
        unionCDA(d,c);
        visualizeCDA(stdout,d);

        printf("\nTesting get and setCDA");
        printf("\nValue returned from getCDA(3) is %d.",*(int*)getCDA(d,3));
        //printf("\nChanging value at 3 from %d to 1.",*(int*)setCDA(d,3,newINT(1)));
        printf("\nValue returned from getCDA(3) is %d.",*(int*)getCDA(d,3));

        printf("\nTesting extractDA\n");
        extractCDA(c);
        visualizeCDA(stdout,c);

        QUEUE *q = newQUEUE(displayINT);
            displayQUEUE(stdout,q);
            printf("\nEnqueue-ing...");
            for(int i = 1; i < 4; ++i) {
            	printf("\n");
            	enqueue(q,newINT(i));
            	displayQUEUE(stdout,q);
            }

            printf("\nPeeking at top value...");
            //void *peekedItemQ = peekQUEUE(q);
            //printf("\nPeeked item is %d.",*(int *)peekedItemQ);

            printf("\nDequeue-ing...");
            void *poppedItemQ = dequeue(q);
            printf("Popped item is %d.",*(int *)poppedItemQ);

            printf("\nQueue size is %d.",sizeQUEUE(q));

            printf("Calling visualizequeue...\n");
            visualizeQUEUE(stdout,q);

    return 0;
}*/
#include <stdio.h>
#include <stdlib.h>
#include "da.h"
#include "int.h"
#include "real.h"
#include "string.h"
extern long random(void);
extern void srandom(unsigned int seed);

int
main(void)
    {
    printf("DA tester: 0,6\n");
    printf("you can retrieve this test with:\n");
    printf("wget beastie.cs.ua.edu/cs201/testing/0/da-test-0-6.c\n");
    printf("\noutput:\n\n");
    DA *d = newDA(displayINT);
    insertDA(d,newINT(548));
    insertDA(d,newINT(344));
    insertDA(d,newINT(340));
    insertDA(d,newINT(867));
    insertDA(d,newINT(875));
    insertDA(d,newINT(807));
    insertDA(d,newINT(46));
    insertDA(d,newINT(31));
    insertDA(d,newINT(583));
    insertDA(d,newINT(413));
    insertDA(d,newINT(883));
    insertDA(d,newINT(821));
    insertDA(d,newINT(35));
    insertDA(d,newINT(806));
    insertDA(d,newINT(964));
    insertDA(d,newINT(329));
    insertDA(d,newINT(839));
    insertDA(d,newINT(856));
    insertDA(d,newINT(674));
    insertDA(d,newINT(655));
    insertDA(d,newINT(881));
    insertDA(d,newINT(895));
    insertDA(d,newINT(349));
    insertDA(d,newINT(693));
    insertDA(d,newINT(909));
    
    printf("array d:");
    visualizeDA(stdout,d);
    printf("\n");
    int size = sizeDA(d);
    printf("extracting the underlying array\n");
    void **a = extractDA(d);
    void *p = a[0];
    printf("the first element of the extracted array: ");
    displayINT(stdout,p);
    printf("\n");
    p = a[size-1];
    printf("the last element of the extracted array: ");
    displayINT(stdout,p);
    printf("\n");
    printf("array d:");
    visualizeDA(stdout,d);
    printf("\n");
    return 0;
    }