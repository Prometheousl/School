CC=gcc
CFLAGS=-g -I.
OPTS = -std=c99
DEPS =
OBJS = awlay.o scanner.o da.o table.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

awlay: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -lm $(OPTS)

clean :
	rm -f $(OBJS)
	rm -f awlay
