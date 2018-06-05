CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99 -pthread
DEPS =
OBJS = pid_manager.o bitmap.o pid_api.o int.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

pid_manager: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

clean :
	rm -f $(OBJS)
	rm -f pid_manager
