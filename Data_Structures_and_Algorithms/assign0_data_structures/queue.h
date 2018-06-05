 #ifndef __QUEUE_INCLUDED__
 #define __QUEUE_INCLUDED__

 #include <stdio.h>

 typedef struct queue QUEUE;

 /**
  * The constructor is passed a function that knows how to display the
  * generic values stored in the queue. That function is
  * stored in a display field of the QUEUE object.
  *
  * @param d = The display function
  * @return the new queue
  */
 extern QUEUE *newQUEUE(void (*d)(FILE *,void *));
 /**
  * The enqueue method runs in constant or amortized constant time.
  * The value to be enqueued is stored in the underlying data structure.
  *
  * @param items = A queue
  * @param value = the value to be inserted into the queue
  */
 extern void enqueue(QUEUE *items,void *value);
 /**
  * The dequeue method runs in constant or amortized constant time.
  * The value to be dequeued is removed in the underlying data structure.
  *
  * @param items = a queue
  * @return the value that is taken from the queue
  */
 extern void *dequeue(QUEUE *items);
 /**
  * The peek method returns the value ready to come off the queue,
  * but leaves the queue unchanged.
  * It runs in constant time.
  *
  * @param items = The queue to be peeked from
  * @return the peeked value
  */
 extern void *peekQUEUE(QUEUE *items);
 /**
  * The size method returns the number of items stored in the queue.
  * It runs in amortized constant time.
  *
  * @param items = The queue to take the size from
  * @return the size of the queue
  */
 extern int sizeQUEUE(QUEUE *items);
 /**
  * This display method prints the items stored in the queue.
  * If the integers 5, 6, 2, 9, and 1 are enqueued in the order given,
  * the method would generate this output:
  * <5,6,2,9,1>
  * with no preceding or following whitespace.
  * An empty queue displays as <>.
  *
  * @param fp = The file to be written to
  * @param items = The queue to display
  */
 extern void displayQUEUE(FILE *fp,QUEUE *items);
 /**
  * The visualize method simply calls the display method
  * of the data structure upon which the queue is based.
  *
  * @param fp = The file to be written to
  * @param items = The queue to visualize
  */
 extern void visualizeQUEUE(FILE *fp,QUEUE *items);
 
 extern int endIndexQUEUE(QUEUE *items);

 #endif
