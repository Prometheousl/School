CC=gcc
CFLAGS=-g -I.
OPTS = -Wall -Wextra -std=c99 -pthread
DEPS =
p320_OBJS = p320.o bitmap.o
p420_OBJS = p420.o bitmap.o pid_api.o int.o
p421_OBJS = p421.o int.o da.o
OBJS = $(p320_OBJS) $(p420_OBJS) $(p421_OBJS)
PROGRAMS = p320 p420 p421

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(OPTS)

all: $(PROGRAMS)

p320: $(p320_OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

p420: $(p420_OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

p421: $(p421_OBJS)
	gcc -o $@ $^ $(CFLAGS) -lm $(OPTS)

clean :
	rm -f $(OBJS)
	rm -f $(PROGRAMS)
