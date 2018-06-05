OPTS = -Wall -Wextra -std=c99
OBJS = da.o cda.o stack.o queue.o int.o real.o string.o
CFLAGS = -c -g

all : $(OBJS)

test : $(OBJS) test-all.o
	gcc $(OPTS) $(OBJS) test-all.o -o test-all
	./test-all

test-all.o : test-all.c da.h cda.h stack.h queue.h int.o real.o string.o
	gcc $(OPTS) $(CFLAGS) test-all.c

da.o : da.c da.h
	gcc $(OPTS) $(CFLAGS) da.c

cda.o : cda.c cda.h
	gcc $(OPTS) $(CFLAGS) cda.c

stack.o : stack.c stack.h da.h
	gcc $(OPTS) $(CFLAGS) stack.c

queue.o : queue.c queue.h cda.h
	gcc $(OPTS) $(CFLAGS) queue.c

int.o : int.c int.h
	gcc $(OPTS) $(CFLAGS) int.c
	
real.o : real.c real.h
	gcc $(OPTS) $(CFLAGS) real.c
	
string.o : string.c string.h
	gcc $(OPTS) $(CFLAGS) string.c

clean :
	rm -f $(OBJS) test-all.o
	rm -f test-all