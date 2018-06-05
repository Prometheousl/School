 #ifndef __QUEUE_INCLUDED__
 #define __QUEUE_INCLUDED__

 #include <stdio.h>

 typedef struct queue QUEUE;

 extern QUEUE *newQUEUE(void (*d)(FILE *,void *));
 extern void enqueue(QUEUE *items,void *value);
 extern void *dequeue(QUEUE *items);
 extern void *peekQUEUE(QUEUE *items);
 extern int sizeQUEUE(QUEUE *items);
 extern void displayQUEUE(FILE *fp,QUEUE *items);
 extern void visualizeQUEUE(FILE *fp,QUEUE *items);
 extern QUEUE *copyQUEUE(QUEUE *queue);

 extern int endIndexQUEUE(QUEUE *items);
 //extern void freeQUEUE(QUEUE *items);

 #endif
