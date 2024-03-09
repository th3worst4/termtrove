#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <ncurses.h>

#define LINE 6

struct cursor{
    unsigned int x;
    unsigned int y;
};
