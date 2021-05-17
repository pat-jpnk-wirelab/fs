#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "fs.h"
#include <libgen.h>
#include <dirent.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>  // defines errno
		            // errno gets set by opendir() from dirent.h


#include <sys/stat.h>
#include <stdlib.h>



int main(void) {

    struct searchItem ss;
    ss.path = "Hey";
    ss.type = REGULAR;
    ss.success = false;
    ss.altered = false;
    

    struct searchIndex si;
    si.size = 0;
    
    struct searchIndex* ii = &si; 

    si.items[0] = ss;

    printf("RES: %s\n", si.items[0].path);

    return 0;
}

void insert(struct searchItem* s_i, uint64_t index) {
    s_i->path = "Hey";
    s_i->type = REGULAR;
    s_i->success = false;
    s_i->altered = false;
}
