char* gochild(DIR* mydir, char* here, char* child, int y, int* success);
char* goparent(DIR* mydir, char* here);
char* findhere(char* here, char* argv0);
char* findhome(char* home, char* here);

struct dirent** printdir(struct dirent** namelist, char* here);
void printcursor(struct cursor *c, int dir);
