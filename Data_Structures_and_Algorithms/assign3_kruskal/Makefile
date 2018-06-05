CC=gcc
CFLAGS=-g
OPTS = -Wall -Wextra -std=c99
DEPS = 
OBJS = kruskal.o set.o scanner.o da.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kruskal: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(OPTS) -lm
	
test : kruskal
	#./trees -v
	#./trees -g ./assign2tester/trees.0 ./assign2tester/trees.1 > test12.me
	#make run-diff
	
run-diff : kruskal
	#diff test12.me ./assign2tester/real/test12
	
full-test : kruskal
	#./trees -g trees.0 trees.1
	#./trees -r trees.0 trees.1
	
clean :
	rm -f $(OBJS)
	rm -f $(TEST_OUTPUT)
	rm -f kruskal
