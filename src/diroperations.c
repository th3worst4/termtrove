#include "includes.h"

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
        // credits for this function: https://stackoverflow.com/a/5467788
        int rv = remove(fpath);

        if (rv)
                perror(fpath);

        return rv;
}

int rmrf(char *path){
        return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

void del(char* here, char* child, int y){
        char* filePath = malloc(strlen(here)+strlen(child));
        strcpy(filePath, here);
        strcat(filePath, child);

        struct stat *fileStat = malloc(sizeof(struct stat));
        stat(filePath, fileStat);

        if(S_ISDIR(fileStat->st_mode)){
                mvaddstr(y, 2+strlen(child)+4, "is a directory, are you sure you want to delete it? y/N");
                int option = getch();
                if(option == 0x79){
                       rmrf(filePath); 
                }else return;
        }else{
                mvaddstr(y, 2+strlen(child)+4, "are you sure you want to delete it? y/N");
                int option = getch();
                if(option == 0x79){
                       remove(filePath);
                }else return;
        }
}

char* gochild(DIR* mydir, char* here, char* child, int y, int* success){
    /**
     * "a function should do only one task"
     * bro, this function does more than the entire rest of the program
    */
    struct stat *newPathStat = malloc(sizeof(struct stat)); 
    char* newPath = malloc(strlen(here)+strlen(child)); //place holder for new path
    
    strcpy(newPath, here);
    strcat(newPath, child); //concat everything under the newPath variable
    strcat(newPath, "/");
    
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
    for(int i = strlen(here)-1; i >= 0; i--){
        if(here[i] == '/' && i != strlen(here) - 1){
            here[i+1] = '\0';
            break;
        }
    }
    here = realloc(here, strlen(here)+1);
    
    mydir = opendir(here); //opens on parent
    
    return here;
}

char* findhere(char* here, char* argv0){
    realpath(argv0, here);
    for(int i = strlen(here)-1; i > 0; i--){
        if(here[i] == '/'){
            here[i+1] = '\0';
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
            home[i+1] = '\0';
            break;
        } 
    }
    home = realloc(home, strlen(home));

    return home;
}


