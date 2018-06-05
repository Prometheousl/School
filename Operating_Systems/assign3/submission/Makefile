CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99 -pthread
DEPS =
p536_OBJS = p536.o bitmap.o pid_api.o int.o
pp3_OBJS = pp3.o buffer.o int.o
OBJS = $(p536_OBJS) $(pp3_OBJS)
PROGRAMS = p536 pp3

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

all: $(PROGRAMS)

p536: $(p536_OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

pp3: $(pp3_OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

clean:
	rm -f $(OBJS)
	rm -f $(PROGRAMS)
