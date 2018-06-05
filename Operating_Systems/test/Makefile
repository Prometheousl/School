CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99
DEPS =
OBJS = test.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

test: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

clean :
	rm -f $(OBJS)
	rm -f test
