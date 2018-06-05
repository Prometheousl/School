CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99
DEPS =
OBJS = awlay.o query.o table.o decision_tree.o
DS_OBJS = data_structures/da.o data_structures/cda.o data_structures/queue.o data_structures/bst.o data_structures/stack.o
INPUT_OBJS = input/scanner.o
CASTING_OBJS = casting/int.o casting/m_string.o casting/conversions.o casting/real.o
TABLE_OBJS =
TEMP = table_data/T1A.o table_data/T1B.o table_data/T2.o
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

awlay: $(OBJS) $(DS_OBJS) $(INPUT_OBJS) $(CASTING_OBJS) $(TABLE_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -lm $(OPTS)

test : awlay
	./awlay
	make run-diff

run-diff : awlay
	diff awlay.txt tests/expected0.txt

clean :
	rm -f $(OBJS)
	rm -f awlay
