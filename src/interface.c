#include "includes.h"

struct dirent** printdir(struct dirent** namelist, char* here){
    erase();
    refresh();
    int l = LINE;
    int n;
    
    n = scandir(here, &namelist, NULL, alphasort);
    
    attron(A_BOLD);
    mvprintw(2, 4, "%s\n", here);
    attroff(A_BOLD);
    
    mvprintw(l-1, 4, "Name");
    for(int i = 1; i < n; i++){
        if(!strcmp(here, "/") && i == 1){
            mvprintw(l, 4, "\n");
            l++;
            refresh();
            continue;
        }
        mvprintw(l, 4, "%s\n", namelist[i]->d_name); 
        l++;
        refresh();
    }

    return namelist;
}

void printcursor(struct cursor *c, int dir){
    switch (dir) {
        case -1:
            break;
        case 0:
            c->x = 2;
            c->y = LINE;
            break;
        case 0x6a: //j
            mvaddch(c->y, c->x, ' '); // clear previous cursor
            c->y++;
            break;
        case 0x6b: //h
            if(c->y <= LINE) return;
            mvaddch(c->y, c->x, ' ');
            c->y--; // bug to fix
            break;
    }
    mvaddch(c->y, c->x, '>');
    refresh();
}
