CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99
DEPS = 
OBJS = matilda.o da.o cda.o stack.o queue.o int.o real.o scanner.o string.o bst.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

matilda: $(OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm
	
test : $(OBJS)
	./matilda -p -b -i mycode-0-7
	
clean :
	rm -f $(OBJS) 
	rm -f matilda