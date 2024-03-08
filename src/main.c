#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <ncurses.h>

struct cursor{
    unsigned int x;
    unsigned int y;
};

char* gochild(DIR* mydir, char* here, char* child){
    closedir(mydir);
    here = realloc(here, strlen(here)+strlen(child));
    strcat(here, "/");
    strcat(here, child);

    mydir = opendir(here);
    
    return here;
}

char* goparent(DIR* mydir, char* here){
    closedir(mydir);
    /**
        * Cannot go futher /home 
        * Needs  analysis
    */
    for(int i = strlen(here)-1; i > 0; i--){
        if(here[i] == '/'){
            here[i] = '\0';
            break;
        }
    }
    here = realloc(here, strlen(here));
    
    mydir = opendir(here); //opens on parent
    
    return here;
}

char* findhere(char* here, char* argv0){
    realpath(argv0, here);
    for(int i = strlen(here)-1; i > 0; i--){
        if(here[i] == '/'){
            here[i] = '\0';
            break;
        }
    }
    here = realloc(here, strlen(here));
    return here;
}

char* findhome(char* home, char* here){
    strcpy(home, here);
    for(int i = 6; i < strlen(here); i++){
        if(home[i] == '/'){
            home[i] = '\0';
            break;
        } 
    }
    home = realloc(home, strlen(home));

    return home;
}

struct dirent** printdir(struct dirent** namelist, char* here){
    erase();
    refresh();
    int line = 4;
    int n;
    
    n = scandir(here, &namelist, NULL, alphasort);
    
    attron(A_BOLD);
    mvprintw(2, 4, "%s\n", here);
    attroff(A_BOLD);
    
    for(int i = 1; i < n; i++){
        mvprintw(line, 4, "%s\n", namelist[i]->d_name); 
        line++;
        refresh();
    }

    return namelist;
}

void printcursor(struct cursor *c, int dir){
    switch (dir) {
        case 0:
            c->x = 2;
            c->y = 4;
            break;
        case 0x6a: //j
            mvaddch(c->y, c->x, ' '); // clear previous cursor
            c->y++;
            break;
        case 0x6b: //h
            if(c->y <= 4) return;
            mvaddch(c->y, c->x, ' ');
            c->y--; // bug to fix
            break;
    }
    mvaddch(c->y, c->x, '>');
    refresh();
}

int main(int argc, char* argv[]){
    char *here = malloc(128*sizeof(char));
    findhere(here, argv[0]);
    
    char *home = malloc(strlen(here)*sizeof(char));
    findhome(home, here);

    void *mydir = NULL;
    struct dirent **namelist;
    
    if(argc > 1 && **(argv+1) == '-'){
          switch (*(*(argv+1)+1)) {
            case 'h': //help messange
                break;
            case 'u': //user, aka. home
                here = home;
                mydir = opendir(here);
                break;
            case 'r': //root
                mydir = opendir("/");
                break;
            default:
                printf("Wrong usage, try -h to help.\n");
                return -1;
                break;
          } 
    }else{
        mydir = opendir(here); //open here
    }
    if(mydir == NULL) return -1; //error handler
  
    bool running = true;
    
    initscr();
    keypad(stdscr, true);
    scrollok(stdscr, true);
    noecho();  

    struct cursor c = {.x = 2, .y = 4};
    
    namelist = printdir(namelist, here);
    printcursor(&c, 0);
    
    while(running){
        int input = getch();
        //mvprintw(0, 0, "%x\n", input);
        switch (input) {
            case 0x0a: // enter??
                /**
                 * return is not working
                 * it should be 0x0d, but the application
                 * does not respond to this input.
                 * need to search more
                 *
                 * wtf is 0x0a
                */
                if(c.y == 4){
                    here = goparent(mydir, here);
                    namelist = printdir(namelist, here);
                    printcursor(&c, 0);  
                }else if(c.y > 4){
                    here = gochild(mydir, here, namelist[c.y-3]->d_name);
                    namelist = printdir(namelist, here);
                    printcursor(&c, 0);  
                } 
                break;
            case 0x6a: //j
                printcursor(&c, 0x6a);
                break;
            case 0x6b: //k
                printcursor(&c, 0x6b);
                break;
            case 0x70: //p
                here = goparent(mydir, here); //get parent folder
                namelist = printdir(namelist, here);
                printcursor(&c, 0);
                break;
                //working unbelievable well
            case 0x71: //q
                running = false;
                break;
        }
        refresh();
    }
    closedir(mydir);
    endwin();
    return 0;
}
