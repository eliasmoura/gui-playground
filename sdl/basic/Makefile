CFLAGS=-std=c99 -Werror -Wextra -Wall -fsanitize=address -fsanitize=undefined -Wno-unused-parameter -I/usr/include/freetype2
LIBS=-lSDL2 -lfreetype -lcairo
BTYPE=debug
CC=clang
ifeq ($(BTYPE), debug)
    CFLAGS+= -g
else
    CFLAGS+= -O2
endif

run: a.out
	./a.out

all:
	$(CC) $(CFLAGS) $(LIBS) clean_window_cairo.c
