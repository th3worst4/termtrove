CC = gcc
CFLAGS = -m64
CLIB = -lncurses -lc

CFILES = main.c diroperations.c interface.c
OBJECTS = main.o diroperations.o interface.o
DOBJECTS = dmain.o ddiroperations.o dinterface.o

$(OBJECTS): %.o: src/%.c
	$(CC) $(CFLAGS) -o bin/$@ -c $^

$(DOBJECTS): %.o: src/%.c
	$(CC) $(CFLAGS) $(CLIB) -g -o bin/$@ -c $^

release: $(OBJECTS)
	$(CC) $(CFLAGS) $(addprefix bin/, $(OBJECTS)) -o bin/main $(CLIB)
	
dbg: $(DOBJECTS)
	$(CC) $(CFLAGS) $(CLIB) -g $(addprefix bin/, $(OBJECTS)) -o bin/main
	gdb bin/main
