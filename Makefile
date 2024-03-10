CC = gcc
CFLAGS = -m64 -lncurses

CFILES = main.c diroperations.c interface.c
OBJECTS = main.o diroperations.o interface.o
DOBJECTS = main.o diroperations.o interface.o

$(OBJECTS): %.o: src/%.c
	$(CC) $(CFLAGS) -o bin/$@ -c $^

$(DOBJECTS): %.o: src/%.c
	$(CC) $(CFLAGS) -g -o bin/$@ -c $^

release: $(OBJECTS)
	$(CC) $(CFLAGS) $(addprefix bin/, $(OBJECTS)) -o bin/main
	
dbg: $(DOBJECTS)
	$(CC) $(CFLAGS) -g $(addprefix bin/, $(OBJECTS)) -o bin/main
	gdb bin/main
