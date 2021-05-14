#include <dirent.h>

int main(void) {
    
    DIR *dirp = opendir(".");
    closedir(dirp);    

    return 0;
}
