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
    struct searchIndex index;
    index.size = 0;
    recursive(ROOTPATH,&index);         // searchIndex populated 
    //printIndex(&index);
    
    struct options options;
    parseIndex(&index, &options);

    return 0;
}

void printIndex(struct searchIndex* index) {
    for(uint64_t k = (index->size - 1); k > 0; k--) {
        printf("RES: %llu > %s > %llu \n", k, index->items[k].path, index->items[k].st_ino);
    }
}

void addToIndex(struct searchIndex* si, const char* item_path, ino_t serial, fileType type) {
    strcpy(si->items[si->size].path, item_path);                       
    si->items[si->size].success = false;                          
    si->items[si->size].st_ino = serial;
    si->items[si->size].type = type;
    si->size++;
}

void recursive(char *path, struct searchIndex* index) {
    struct dirent *dp;
    DIR *dir = opendir(path);

    while ((dp = readdir(dir)) != NULL) {
        char item_path[MAX_PATH_SIZE] = "";                                             // pathname 
        char item_path2[MAX_PATH_SIZE] = "";                                            // gets '/' added if directory, used for next recursion
        const char* item_name = dp->d_name;

        if(strcmp(item_name, ".") != 0 && strcmp(item_name, "..") != 0) {               // gets also checked in getItemPath();
            strcat(item_path,path);                                                                  
            strcat(item_path, item_name);
            fileType type = getFileStatus(item_path);
            getItemPath(path, item_name, &item_path2[0], type);

            switch(type) {
                case REGULAR:
                    addToIndex(index, item_path, dp->d_ino, type);
                    break;
                case DIREC:
                    if ((strcmp(item_path, "./.") != 0 && strcmp(item_path, "./..") != 0) && strcmp(item_path, "./.git") != 0 && (int) type == 4) {
                        recursive(item_path2, index);
                    }
                    break;
                default:
                    break;
            }
        }
    } 
    closedir(dir);
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
    switch(fileStatusRes) {
        case 0:
            //printf("success\n");
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
 * @return struct searchItem - item
 **/ 
void createSearchItem(struct searchItem* item, ino_t serial, char* path, fileType type) {               
    //item->path = path;
    strcpy(item->path, path);
    item->success = false;    // default false at this point, no search attempted thus far
    item->st_ino = serial;
    item->type = type;
}


/**
 * @param void
 * @return struct searchStats - new struct to hold search statistics
**/
struct searchStats initSearchStats() {
    struct searchStats stats;
    stats.file_count = 0;
    stats.dir_count = 0;
    stats.search_count = 0;
    stats.alter_count = 0;
    return stats;
}

/**
 * @param const char* path      - current base path
 * @param const char* item_name - item name
 * @return const char*
 **/
void getItemPath(const char* path, const char* item_name, char* item_path, fileType type) {                   
    strcat(item_path,path);                                                                        
    strcat(item_path, item_name); 
    if(strcmp(item_name, ".") != 0 && strcmp(item_name, "..") != 0 && strcmp(item_name, ".git") != 0 && type == DIREC) {
        strcat(item_path, "/");
    }                                                         
}
 
void parseFile(operation op, struct searchIndex* index, struct options options, uint64_t size) {
    for (uint64_t i = 0; i < size; i++) {
        op(&(index->items[i]), options);
    }
}

void parseIndex(struct searchIndex* index, struct options* options) {
    switch(options->function) {
        case SEARCH:
            parseFile(_search,index,*options,index->size);
            break;
        case REPLACE:
            break;
        default:
            break;
    }
}


// if operation: SEARCH -> _search function = options[bool capitalization;, bool spacing, char* search_term;]
void _search(struct searchItem* item, struct options options) {
    printf("Hello World from search\n");
    // capitalization, spacing, search_term
    
    /**
    FILE *fp = fopen(item->path, "r");
    uint32_t c;

    while(true) {
        c = fgetc(fp);
        if(feof(fp)){
            break;
        }
        printf("%c", c);
    }
    fclose(fp);

    **/
}


// if operation: REPLACE -> _replace function = options[bool capitalization;, bool spacing, char* search_term; char* replacement_term]
void _replace(struct searchItem* item, struct options options) {
    printf("Hello world from replace");
    return;
}