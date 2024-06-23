CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g

SRC = main.c
NAME = main

all: libbst $(NAME)

libbst:
	@if [ ! -f libbst.a ]; then \
		$(MAKE) -C include; \
	fi

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -lm -o $(NAME) -Linclude -lbst

clean:
	@$(MAKE) -C include clean
	@rm $(NAME)
