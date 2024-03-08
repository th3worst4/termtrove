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

char* gochild(DIR* mydir, char* here, char* child, int y, int* success){
    /**
     * "a function should do only one task"
     * bro, this function does more than the entire rest of the program
    */
    struct stat *newPathStat = malloc(sizeof(struct stat)); 
    char* newPath = malloc(strlen(here)+strlen(child)); //place holder for new path
    
    strcpy(newPath, here);
    strcat(newPath, "/");
    strcat(newPath, child); //concat everything under the newPath variable
    
    stat(newPath, newPathStat); //uses it to verify if the newPath is a directory or not

    if(S_ISDIR(newPathStat->st_mode)){ //S_ISDIR is the macro that verifies it
        closedir(mydir);
        mydir = opendir(newPath);
        *success = 1;
        here = realloc(here, strlen(newPath)+1); //must realloc on here, 'cause newPath data will be gone as the function exits
        strcpy(here, newPath);
        return here; //returns the newPath if the path is a directory
    }else{
        mvaddstr(y, 2+strlen(child)+4, "You cannot open non directories");
        refresh();
        *success = 0;
        return here; 
    } 
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
    int l = LINE;
    int n;
    
    n = scandir(here, &namelist, NULL, alphasort);
    
    attron(A_BOLD);
    mvprintw(2, 4, "%s\n", here);
    attroff(A_BOLD);
    
    mvprintw(l-1, 4, "Name");
    for(int i = 1; i < n; i++){
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

    struct cursor c = {.x = 2, .y = LINE};
    
    namelist = printdir(namelist, here);
    printcursor(&c, 0);
    
    while(running){
        int input = getch();
        int success;
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
                if(c.y == LINE){
                    here = goparent(mydir, here);
                    namelist = printdir(namelist, here);
                    printcursor(&c, 0);  
                }else if(c.y > LINE){
                    here = gochild(mydir, here, namelist[c.y-(LINE-1)]->d_name, c.y, &success);
                    if(success){ //checks if new path is a folder
                        namelist = printdir(namelist, here);
                        printcursor(&c, 0);  
                    }else{
                        printcursor(&c, -1); 
                    }
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
