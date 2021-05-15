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

int main(void) {                                                 // TODO: int argc, char *argv[]

    struct searchItem items[MAX_SEARCH_ITEMS]; 
                                                                 // TODO: should this be const ?
    struct searchStats stats = createSearchStats();                                 // TODO: should this be const ?
    struct searchIndex index = createSearchIndex(&items[0]);                        // TODO: should this be const ?

    char* pp = "./";
    
    recursive(pp,&index);

    printf("Search index LENGTH: %llu\n", index.size);

    return 0;
}

/**
 * from stackoverflow (https://stackoverflow.com/questions/56212434/how-to-know-a-files-type)
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


/**
 * @param pathname of file 
 * @returns fileType
**/
fileType getFileStatus (const char* path) {

        //struct stat* buffer = malloc(sizeof(stat)); 
    struct stat buff;
    struct stat* buffer = &buff;

    int fileStatusRes = stat(path,buffer); // indicates whether it was gotten successfully 
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

    fileType fileType = FileType(buffer->st_mode);
        //free(buffer);
    return fileType;
}


/**
 * @param 
 * @return int 
 **/

/**
int parseDirectory(const char* path ,struct searchIndex* index, struct dirIndex* d_index) {
    DIR *rootDirectory = opendir(path);  
    struct dirent *currentDirectoryEntry;                                                        // TODO: catch opendir error

    while((currentDirectoryEntry = readdir(rootDirectory)) != NULL) {                              // TODO: check readdir errors
        
        //printf("RES: %s\n", currentDirectoryEntry->d_name);

        char item_path[MAX_PATH_SIZE] = "";
        const char* item_name = currentDirectoryEntry->d_name;
        fileType type = getFileStatus(item_path);

        switch(type) {
            case REGULAR:
                ;
                struct searchItem item = createSearchItem(currentDirectoryEntry->d_ino,item_path,type); 
                //addToSearchIndex(item, index);
                break;
            
            case DIREC:
                if(strcmp(item_name, ".") == 0) {
                    printf("ONE \n");
                    break;
                } else if (strcmp(item_name, "..") == 0) {
                    //printf("BREAK2 %s\n", name);
                    printf("TWO \n");
                    break;
                } else if (strcmp(item_name, ".git") == 0) {
                    //printf("BREAK2 %s\n", name);
                    printf("THREE \n");
                    break;
                } else {
                    printf("BREAK3 %s\n", item_path);
                    dirItem item = createDirItem(item_path);
                    //addToDirIndex(item,d_index);
                    break;
                }
            default:
                break;
        }    
    }

    //closedir(rootDirectory);
    /**
    uncommenting this causes 
        a.out(1490,0x10d7815c0) malloc: Incorrect checksum for freed object 0x7fba144006a8: probably modified after being freed.
        Corrupt value: 0x0
        a.out(1490,0x10d7815c0) malloc: *** set a breakpoint in malloc_error_break to debug
        Abort trap: 6

    if (close == -1) {
        exit(1);
    }
    **/

/**
    //int close = closedir(rootDirectory);
    printf("Dir index size: %llu \n",d_index->size);
    return 0;
}
**/

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
    index.items = items;                       // TODO: IS THIS CORRECT ?? should it be a pointer ? why ? 

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
int addToSearchIndex(struct searchItem item, struct searchIndex* index) {
    index->items[index->size++] = item;
    return 0;
}


/**
 * @param const char* path      - current base path
 * @param const char* item_name - item name
 * @return const char*
 **/
void getItemPath(const char* path, const char* item_name, char* item_path, fileType type) {                        //  path: ./  
    strcat(item_path,path);                                                                         //  item_name: testDirectory
    strcat(item_path, item_name); 

    // this fails, adds '/' to ascii files ??
    if(strcmp(item_name, ".") != 0 && strcmp(item_name, "..") != 0 && strcmp(item_name, ".git") != 0 && type == DIREC) {
        strcat(item_path, "/");
    }
    printf("\t\t\t%s \n", item_path);                                                                  // "./testDirectory"
}

void recursive(char *path, struct searchIndex* index) {
    struct dirent *dp;
    DIR *dir = opendir(path);

    //printf("READING: %s\n", path);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL) {

        char item_path[MAX_PATH_SIZE] = "";
        char item_path2[MAX_PATH_SIZE] = "";
        const char* item_name = dp->d_name;                 


        strcat(item_path,path);                                                                  
        strcat(item_path, item_name);
        
        fileType type = getFileStatus(item_path);


        getItemPath(path, item_name, &item_path2[0], type); 


        if(strcmp(item_name,".") != 0 && strcmp(item_name,"..") != 0) {
            if ( (strcmp(item_path, "./.") != 0 && strcmp(item_path, "./..") != 0) && strcmp(item_path, "./.git") != 0 && (int) type == 4) {
                recursive(item_path2, index);
            }
        }

        
        switch(type) {
            case REGULAR:
                ;
                struct searchItem item = createSearchItem(dp->d_ino,item_path,type); 
                addToSearchIndex(item, index);
                break;
            
            case DIREC:
                if(strcmp(item_name,".") != 0 && strcmp(item_name,"..") != 0) {
                   if ( (strcmp(item_path, "./.") != 0 && strcmp(item_path, "./..") != 0) && strcmp(item_path, "./.git") != 0 && (int) type == 4) {
                        recursive(item_path2, index);
                    }
                }
                break;
            /**
                if(strcmp(item_name, ".") == 0) {
                    printf("ONE \n");
                    break;
                } else if (strcmp(item_name, "..") == 0) {
                    //printf("BREAK2 %s\n", name);
                    printf("TWO \n");
                    break;
                } else if (strcmp(item_name, ".git") == 0) {
                    //printf("BREAK2 %s\n", name);
                    printf("THREE \n");
                    break;
                } else {
                    printf("BREAK3 %s\n", item_path);
                    dirItem item = createDirItem(item_path);
                    addToDirIndex(item,d_index);
                    break;
                }
                **/
            default:
                break;
        }

    }
    
    closedir(dir);
}