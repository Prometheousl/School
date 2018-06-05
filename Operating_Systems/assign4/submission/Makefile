CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99 -pthread
DEPS =
OBJS = dispatcher.o scanner.o rbt.o process.o bst.o queue.o cda.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

dispatcher: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -lm $(OPTS)
	gcc sigtrap.c -o process -Wall

clean :
	rm -f $(OBJS)
	rm -f dispatcher
	rm -f process
