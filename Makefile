CC = gcc
CFLAGS = -m64 -lncurses -lc -g

CFILES = main.c
OBJECTS = main.o
BIN = main

$(OBJECTS): %.o: src/%.c
	$(CC) $(CFLAGS) -o bin/$@ -c $^

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(addprefix bin/, $(OBJECTS)) -o bin/main
