CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99
DEPS =
OBJS = pid_api.o bitmap.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

pid_api: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

clean :
	rm -f $(OBJS)
	rm -f pid_api
