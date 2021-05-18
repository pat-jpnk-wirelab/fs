#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "fs.h"
#include "operations.h"
#include <libgen.h>
#include <dirent.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>      // defines errno
		                // errno gets set by opendir() from dirent.h
#include <sys/stat.h>
#include <stdlib.h>



// make check function to ignore certain folders 
// ".", "..", ".gitignore"


int main(void) {
    searchStats stats = initSearchStats();
    
    struct options options;
    options.function = SEARCH;

    struct searchIndex index;
    index.size = 0;
    
    recursive(ROOTPATH, &index, &stats);         // searchIndex populated when this finishes
    
    //printIndex(&index);
    
    parseIndex(&index, &options);
    
    //printStats(&stats);
    
    return 0;
}

void addToIndex(struct searchIndex* si, const char* item_path, ino_t serial, fileType type) {
    strcpy(si->items[si->size].path, item_path);                       
    si->items[si->size].success = false;                          
    si->items[si->size].st_ino = serial;
    si->items[si->size].type = type;
    si->size++;
}

void recursive(char *path, struct searchIndex* index, struct searchStats* stats) {
    struct dirent *dp;
    DIR *dir = opendir(path);

    while ((dp = readdir(dir)) != NULL) {
        char item_path[MAX_PATH_SIZE] = "";                                             // pathname 
        char item_path2[MAX_PATH_SIZE] = "";                                            // gets '/' added if directory, used for next recursion // TODO: add better name
        const char* item_name = dp->d_name;

        if(strcmp(item_name, ".") != 0 && strcmp(item_name, "..") != 0) {               // gets also checked in getItemPath();
            strcat(item_path,path);                                                                  
            strcat(item_path, item_name);
            fileType type = getFileStatus(item_path);
            getItemPath(path, item_name, &item_path2[0], type);

            switch(type) {
                case REGULAR:
                    stats->file_count++;                                // update stats
                    printf("COUNTING: %s\n", item_name);
                    addToIndex(index, item_path, dp->d_ino, type);
                    break;
                case DIREC:
                    if ((strcmp(item_path, "./.") != 0 && strcmp(item_path, "./..") != 0) && strcmp(item_path, "./.git") != 0 && (int) type == 4) {
                        stats->dir_count++;                             // update stats
                        recursive(item_path2, index, stats);
                    }
                    break;
                default:
                    break;
            }
        }
    } 
    closedir(dir);
}


fileType FileType (mode_t m) {              // @param m | st_mode attribute of struct stat
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


fileType getFileStatus (const char* path) {
    struct stat buff;
    struct stat* buffer = &buff;
    fileType fileType;

    int fileStatusRes = stat(path,buffer); // indicates whether it was gotten successfully => success = 0, failure = -1
    
    if(fileStatusRes == -1) {
        fileType = UNKNOWN;
    } else {
        fileType = FileType(buffer->st_mode);
    }
    
    return fileType;
}

 
void createSearchItem(struct searchItem* item, ino_t serial, char* path, fileType type) {
    strcpy(item->path, path);
    item->success = false;    // default false at this point, no search attempted thus far
    item->st_ino = serial;
    item->type = type;
}


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
            printf("REPLACE!\n");
            parseFile(_replace,index,*options, index->size);
            break;
        default:
            break;
    }
}


// ############## helper #################
void printIndex(struct searchIndex* index) {
    for(uint64_t k = (index->size - 1); k > 0; k--) {
        printf("RES: %llu > %s > %llu \n", k, index->items[k].path, index->items[k].st_ino);
    }
}

void printStats(struct searchStats* stats) {
    printf("\nstats:\nf: %llu\nd: %llu\ns: %llu\na: %llu\n", stats->file_count, stats->dir_count, stats->search_count, stats->alter_count);
}
// ######################################## 