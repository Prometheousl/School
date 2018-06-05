CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99 -pthread
DEPS =
OBJS = statistics.o int.o da.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

statistics: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

clean :
	rm -f $(OBJS)
	rm -f statistics
