CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99
DEPS =
FIFO_OBJS = scanner.o address.o page_table.o disk.o da.o entry.o int.o tlb.o cda.o queue.o fifo.o
LRU_OBJS = scanner.o address.o modified_page_table.o disk.o da.o modified_entry.o int.o tlb.o cda.o queue.o lru.o
PROGRAMS = fifo lru
TEST_OUTPUT = *.me

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

all: $(PROGRAMS)

fifo: $(FIFO_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -lm $(OPTS)

lru: $(LRU_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) -lm $(OPTS)

clean :
	rm -f $(FIFO_OBJS)
	rm -f $(LRU_OBJS)
	rm -f $(PROGRAMS)
