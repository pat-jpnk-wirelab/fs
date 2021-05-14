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

int main(int argc, char *argv[]) {

    /**
    
    char k[] = "/tier/mensch/Hello.v";

    int64_t num = 313432;

    // limits.h OPEN_MAX :  Maximum number of files that one process can have open at any one time. Minimum Acceptable Value: 20	
    printf("open max: %d\n", OPEN_MAX);

    printf("%s\n",basename(k)); // filename
    printf("%s\n", dirname(k)); // pathname

    **/
    // ###########################################

    struct searchIndex index[MAX_SEARCH_ITEMS];
    struct searchStats stats;



    // Parse directories, starting with ROOTPATH


    parseDirectory(ROOTPATH, &index);


    // ------



    //int close = closedir(rootDirectory); 
    /**
    uncommenting this causes 
        a.out(1490,0x10d7815c0) malloc: Incorrect checksum for freed object 0x7fba144006a8: probably modified after being freed.
        Corrupt value: 0x0
        a.out(1490,0x10d7815c0) malloc: *** set a breakpoint in malloc_error_break to debug
        Abort trap: 6
    **/
    /**
    if (close == -1) {
        exit(1);
    }
    **/

    return 0;
}

/**
 * from stackoverflow (https://stackoverflow.com/questions/56212434/how-to-know-a-files-type)
 * 
 * @param m | st_mode attribute of struct stat
 * @retuns char 
**/
fileType FileType (mode_t m) {
    switch (m & S_IFMT) {                   //bitwise AND to determine file type
        case S_IFSOCK:  return SOCKET;      //socket
        case S_IFLNK:   return SYMLINK;     //symbolic link
        case S_IFREG:   return REGULAR;     //regular file
        case S_IFBLK:   return BLOCK;       //block device
        case S_IFDIR:   return DIREC;       //directory
        case S_IFCHR:   return CHAR;        //char device
        case S_IFIFO:   return PIPE;        //pipe
        default: return UNKNOWN;            //unknown
    }
}




// int stat(const char *restrict path, struct stat *restrict buf);

/**
 * @param pathname of file 
 * @returns int
**/
int getFileStatus (const char* path) {

    struct stat* buffer = malloc(sizeof(stat)); 

    int fileStatusRes = stat(path,buffer); // idicates whether inf owas gotten successfully 
    // success = 0, failure = -1

    switch(fileStatusRes) {
        case 0:
            printf("success\n");
            break; 
        default:
            printf("fail\n");
            break;
    }

    printf("filetype: %c\n\n", FileType(buffer->st_mode));     // now returns enum

    
    free(buffer);

    return 0;
}


// param:  

int parseDirectory(const char* path ,struct searchIndex* index[]) {                          // TODO: call recursively when detecting a directory
    DIR *rootDirectory;
    struct dirent *currentDirectory;
    
    rootDirectory = opendir(ROOTPATH);


    while((currentDirectory = readdir(rootDirectory)) != NULL) {
        printf("RES: %s\n", currentDirectory->d_name);

        const char* src = currentDirectory->d_name;


        // ###################################################
        // build file path name 

        // char *strcat(char *dest, const char *src)
        char path[256] = "";
        char* path_ptr = &path[0];      // remove [0] for nice errors

        strcat(path_ptr,ROOTPATH);
        strcat(path_ptr, src);
        printf("path: %s\n", path);
        // ###################################################

        const char* _path = &path[0]; 

        getFileStatus(_path);

       // printf("%d %d\n", fileStatusRes, errno);	    
	   // printf("%c\n", FileType(fileStatus->st_mode));        
    }




    return 0;
}