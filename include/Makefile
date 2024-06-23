CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g
LDFLAGS = -lm

SRCS = $(wildcard *.c)
OJBS = $(patsubst %.c, %.o, $(SRCS))

LIB = libbst.a

all: $(LIB)

$(LIB): $(OJBS)
	ar rcs $@ $^ $(LDFLAGS)

$(OJBS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS) -o $@

clean:
	rm $(OJBS) $(LIB)
