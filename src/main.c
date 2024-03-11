#include "includes.h"
#include "functions.h"

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
                strcpy(here, home);
                mydir = opendir(here);
                break;
            case 'r': //root
                strcpy(here, "/");
                mydir = opendir(here);
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
                if(c.y == LINE && strcmp(here, "/")){
                    here = goparent(mydir, here);
                    refresh();
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
            case 0x64: //d 
                del(here, namelist[c.y-(LINE-1)]->d_name, c.y);
                namelist = printdir(namelist, here);
                printcursor(&c, 0);
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
