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

int main(void) {                            // TODO: int argc, char *argv[]

    /**
    
    char k[] = "/tier/mensch/Hello.v";

    int64_t num = 313432;

    // limits.h OPEN_MAX :  Maximum number of files that one process can have open at any one time. Minimum Acceptable Value: 20	
    printf("open max: %d\n", OPEN_MAX);

    printf("%s\n",basename(k)); // filename
    printf("%s\n", dirname(k)); // pathname

    **/
    // ###########################################

    struct searchItem items[MAX_SEARCH_ITEMS];                                      // TODO: should this be const ?

    struct searchStats stats = createSearchStats();                                 // TODO: should this be const ?
    struct searchIndex index = createSearchIndex(&items[0]);                        // TODO: should this be const ?



    // Parse directories, starting with ROOTPATH


    parseDirectory(ROOTPATH, &index);


    // ------



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
fileType getFileStatus (const char* path) {

    struct stat* buffer = malloc(sizeof(stat)); 

    int fileStatusRes = stat(path,buffer); // idicates whether inf owas gotten successfully 
    // success = 0, failure = -1

    // TODO: add real actions upon failure 
    switch(fileStatusRes) {
        case 0:
            printf("success\n");
            break; 
        default:
            printf("fail\n");
            break;
    }

    fileType ft = FileType(buffer->st_mode);

    free(buffer);

    return ft;
}


// param:  


/**
 * @param 
 * @return int 
 **/
int parseDirectory(const char* path ,struct searchIndex* index) {                          // TODO: call recursively when detecting a directory
    DIR *rootDirectory;
    struct dirent *currentDirectoryEntry;
    
    rootDirectory = opendir(path);                                                          // TODO: catch opendir error


    while((currentDirectoryEntry = readdir(rootDirectory)) != NULL) {                              // TODO: check readdir errors
        
        // HERE WE GOT A FILE 
        
        printf("RES: %s\n", currentDirectoryEntry->d_name);                          

        const char* item_name = currentDirectoryEntry->d_name;

        const char* _path = getItemPath(path, item_name);

        fileType type = getFileStatus(_path);

        switch(type) {
            case REGULAR:
                printf("REGULAR");
                // int addToIndex(struct searchItem item, struct searchIndex* index)
                // struct searchItem createSearchItem(ino_t serial, char* path, fileType type)
                struct searchItem item = createSearchItem(currentDirectoryEntry->d_ino,_path,type); 
                addToIndex(item, index);
                printf("\n!!! index size: %llu!!!\n\n", index->size);
                break;
            
            case DIREC:
                printf("DIRECTORY");
                break;
            default:
                break;
        }


       // printf("%d %d\n", fileStatusRes, errno);	    
	   // printf("%c\n", FileType(fileStatus->st_mode));        
    }


    //closedir(rootDirectory);
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
 * @param
 * @return struct searchItem - item
 **/ 
struct searchItem createSearchItem(ino_t serial, char* path, fileType type) {
    struct searchItem item;
    item.success = false;    // default false at this point, no search attempted thus far
    item.st_ino = serial;
    item.type = type;

    return item;
}

/**
 * @param struct 
 * @return struct searchIndex 
 **/
struct searchIndex createSearchIndex(struct searchItem* items) {
    struct searchIndex index;
    index.size = 0;                             // starts with size 0
    index.items = items;                       // TODO: IS THIS CORRECT ??

    return index;
}

/**
 * @param void
 * @return struct searchStats - new struct to hold search statistics
**/
struct searchStats createSearchStats() {
    struct searchStats stats;
    stats.f_count = 0;
    stats.d_count = 0;
    return stats;
}


/**
 * @param item  - new item to add to index
 * @param index - index to add item to 
 * @return 0 = success | 1 = failure
 **/
int addToIndex(struct searchItem item, struct searchIndex* index) {
    index->items[index->size++] = item;
    return 0;
}

/**
 * @param const char* path      - current base path
 * @param const char* item_name - item name
 * @return const char*
 **/
const char* getItemPath(const char* path, const char* item_name) {
    char item_path[MAX_PATH_SIZE] = "";
    char* path_ptr = &item_path[0];      // remove [0] for nice errors

    strcat(path_ptr,path);
    strcat(path_ptr, item_name);

    return path_ptr;
}