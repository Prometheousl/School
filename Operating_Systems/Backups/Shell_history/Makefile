CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99
DEPS =
OBJS = shell_history.o da.o stack.o my_string.o str_array.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

shell: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

clean :
	rm -f $(OBJS)
	rm -f shell
