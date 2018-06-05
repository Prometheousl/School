CC=gcc
CFLAGS=-g
OPTS = -Wall -Wextra -std=c99
DEPS = 
OBJS = trees.o queue.o string.o bst.o cda.o rbt.o gt.o scanner.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

trees: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(OPTS) -lm
	
test : trees
	./trees -v
	#./trees -g ./assign2tester/trees.0 ./assign2tester/trees.1 > test12.me
	#./trees -r ./assign2tester/trees.0 ./assign2tester/trees.1 > test13.me
	#./trees -g ./assign2tester/test-0-0.corpus ./assign2tester/test-0-0.go > test14.me
	#./trees -r ./assign2tester/test-0-0.corpus ./assign2tester/test-0-0.go > test15.me
	#./trees -g ./assign2tester/test-0-1.corpus ./assign2tester/test-0-1.go > test16.me
	#./trees -r ./assign2tester/test-0-1.corpus ./assign2tester/test-0-1.go > test17.me
	#./trees -g ./assign2tester/test-0-2.corpus ./assign2tester/test-0-2.go > test18.me
	#./trees -r ./assign2tester/test-0-2.corpus ./assign2tester/test-0-2.go > test19.me
	#./trees -g ./assign2tester/test-0-3.corpus ./assign2tester/test-0-3.go > test20.me
	#./trees -r ./assign2tester/test-0-3.corpus ./assign2tester/test-0-3.go > test21.me
	#./trees -g ./assign2tester/test-0-4.corpus ./assign2tester/test-0-4.go > test22.me
	#./trees -r ./assign2tester/test-0-4.corpus ./assign2tester/test-0-4.go > test23.me
	#./trees -g ./assign2tester/test-0-5.corpus ./assign2tester/test-0-5.go > test24.me
	#./trees -r ./assign2tester/test-0-5.corpus ./assign2tester/test-0-5.go > test25.me
	#./trees -g ./assign2tester/test-0-6.corpus ./assign2tester/test-0-6.go > test26.me
	#./trees -r ./assign2tester/test-0-6.corpus ./assign2tester/test-0-6.go > test27.me
	#./trees -g ./assign2tester/test-0-7.corpus ./assign2tester/test-0-7.go > test28.me
	#./trees -r ./assign2tester/test-0-7.corpus ./assign2tester/test-0-7.go > test29.me
	#./trees -g ./assign2tester/test-0-8.corpus ./assign2tester/test-0-8.go > test30.me
	#./trees -r ./assign2tester/test-0-8.corpus ./assign2tester/test-0-8.go > test31.me
	#./trees -g ./assign2tester/test-0-9.corpus ./assign2tester/test-0-9.go > test32.me
	#./trees -r ./assign2tester/test-0-9.corpus ./assign2tester/test-0-9.go > test33.me
	#./trees -g ./assign2tester/test-0-10.corpus ./assign2tester/test-0-10.go > test34.me
	#./trees -r ./assign2tester/test-0-10.corpus ./assign2tester/test-0-10.go > test35.me
	#./trees -g ./assign2tester/test-0-11.corpus ./assign2tester/test-0-11.go > test36.me
	#./trees -r ./assign2tester/test-0-11.corpus ./assign2tester/test-0-11.go > test37.me
	#./trees -g ./assign2tester/test-0-12.corpus ./assign2tester/test-0-12.go > test38.me
	#./trees -r ./assign2tester/test-0-12.corpus ./assign2tester/test-0-12.go > test39.me
	#./trees -g ./assign2tester/test-0-13.corpus ./assign2tester/test-0-13.go > test40.me
	./trees -r ./assign2tester/test-0-13.corpus ./assign2tester/test-0-13.go > test41.me
	./trees -g ./assign2tester/test-0-14.corpus ./assign2tester/test-0-14.go > test42.me
	./trees -r ./assign2tester/test-0-14.corpus ./assign2tester/test-0-14.go > test43.me
	./trees -g ./assign2tester/beowulf.txt ./assign2tester/beowulf.go1 > test44.me
	./trees -r ./assign2tester/beowulf.txt ./assign2tester/beowulf.go1 > test45.me
	./trees -g ./assign2tester/beowulf.txt ./assign2tester/beowulf.go2 > test46.me
	./trees -r ./assign2tester/beowulf.txt ./assign2tester/beowulf.go2 > test47.me
	./trees -g ./assign2tester/shakespeare.txt ./assign2tester/shakespeare.go1 > test48.me
	./trees -r ./assign2tester/shakespeare.txt ./assign2tester/shakespeare.go1 > test49.me
	./trees -g ./assign2tester/shakespeare.txt ./assign2tester/shakespeare.go2 > test50.me
	./trees -r ./assign2tester/shakespeare.txt ./assign2tester/shakespeare.go2 > test51.me
	make run-diff
	
run-diff : trees
	diff test12.me ./assign2tester/real/test12
	diff test13.me ./assign2tester/real/test13
	diff test14.me ./assign2tester/real/test14
	diff test15.me ./assign2tester/real/test15
	diff test16.me ./assign2tester/real/test16
	diff test17.me ./assign2tester/real/test17
	diff test18.me ./assign2tester/real/test18
	diff test19.me ./assign2tester/real/test19
	diff test20.me ./assign2tester/real/test20
	diff test21.me ./assign2tester/real/test21
	diff test22.me ./assign2tester/real/test22
	diff test23.me ./assign2tester/real/test23
	diff test24.me ./assign2tester/real/test24
	diff test25.me ./assign2tester/real/test25
	diff test26.me ./assign2tester/real/test26
	diff test27.me ./assign2tester/real/test27
	diff test28.me ./assign2tester/real/test28
	diff test29.me ./assign2tester/real/test29
	diff test30.me ./assign2tester/real/test30
	diff test31.me ./assign2tester/real/test31
	diff test32.me ./assign2tester/real/test32
	diff test33.me ./assign2tester/real/test33
	diff test34.me ./assign2tester/real/test34
	diff test35.me ./assign2tester/real/test35
	diff test36.me ./assign2tester/real/test36
	diff test37.me ./assign2tester/real/test37
	diff test38.me ./assign2tester/real/test38
	diff test39.me ./assign2tester/real/test39
	diff test40.me ./assign2tester/real/test40
	diff test41.me ./assign2tester/real/test41
	diff test42.me ./assign2tester/real/test42
	diff test43.me ./assign2tester/real/test43
	diff test44.me ./assign2tester/real/test44
	diff test45.me ./assign2tester/real/test45
	diff test46.me ./assign2tester/beowulf-g.tree
	diff test47.me ./assign2tester/beowulf-r.tree
	diff test48.me ./assign2tester/real/test48
	diff test49.me ./assign2tester/real/test49
	diff test50.me ./assign2tester/shakespeare-g.tree
	diff test51.me ./assign2tester/shakespeare-r.tree
	
full-test : trees
	./trees -g trees.0 trees.1
	./trees -r trees.0 trees.1
	./trees -g test-0-0.corpus test-0-0.go
	./trees -r test-0-0.corpus test-0-0.go
	./trees -g test-0-1.corpus test-0-1.go
	./trees -r test-0-1.corpus test-0-1.go
	./trees -g test-0-2.corpus test-0-2.go
	./trees -r test-0-2.corpus test-0-2.go
	./trees -g test-0-3.corpus test-0-3.go
	./trees -r test-0-3.corpus test-0-3.go
	./trees -g test-0-4.corpus test-0-4.go
	./trees -r test-0-4.corpus test-0-4.go
	./trees -g test-0-5.corpus test-0-5.go
	./trees -r test-0-5.corpus test-0-5.go
	./trees -g test-0-6.corpus test-0-6.go
	./trees -r test-0-6.corpus test-0-6.go
	./trees -g test-0-7.corpus test-0-7.go
	./trees -r test-0-7.corpus test-0-7.go
	./trees -g test-0-8.corpus test-0-8.go
	./trees -r test-0-8.corpus test-0-8.go
	./trees -g test-0-9.corpus test-0-9.go
	./trees -r test-0-9.corpus test-0-9.go
	./trees -g test-0-10.corpus test-0-10.go
	./trees -r test-0-10.corpus test-0-10.go
	./trees -g test-0-11.corpus test-0-11.go
	./trees -r test-0-11.corpus test-0-11.go
	./trees -g test-0-12.corpus test-0-12.go
	./trees -r test-0-12.corpus test-0-12.go
	./trees -g test-0-13.corpus test-0-13.go
	./trees -r test-0-13.corpus test-0-13.go
	./trees -g test-0-14.corpus test-0-14.go
	./trees -r test-0-14.corpus test-0-14.go
	./trees -g beowulf.txt beowulf.go1
	./trees -r beowulf.txt beowulf.go1
	./trees -g beowulf.txt beowulf.go2
	./trees -r beowulf.txt beowulf.go2
	./trees -g shakespeare.txt shakespeare.go1
	./trees -r shakespeare.txt shakespeare.go1
	./trees -g shakespeare.txt shakespeare.go2
	./trees -r shakespeare.txt shakespeare.go2
	
clean :
	rm -f $(OBJS)
	rm -f $(TEST_OUTPUT)
	rm -f trees
