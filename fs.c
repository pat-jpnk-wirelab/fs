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

#include <getopt.h>
#include "colors.h"



// ".", ".."  ignored by default 
// other file names /  directory names / file extensions  => can be manually ignored
/**
 * - filenames to ignore                    function 
 * - directories to ignore 
 * - files with certain extensions to ignore
 * 
 **/
int main(int argc, char *argv[]) {
    int opt;
    struct options options;
    options.search_term = "special";


    while((opt = getopt(argc, argv, "s:rih")) != -1) {
        switch(opt) {
            case 's':
                printf("ARG: %s\n", optarg);  // argv[2]
                options.function = SEARCH;
                printf(KGRN"OPTION search chosen!\n"KRESET);
                break;

            case 'r':
                options.function = REPLACE;
                printf(KCYN"OPTION replace chosen!\n"KRESET);
                break;

            case 'i':
                options.function = INFO;
                printf(KYEL"OPTION info chosen!\n"KRESET);
                break;
            
            case 'h':
                // give help
                printf("halp\n");
                return 0;
            default:
                break;
        }   
    }


   // char ignore_file[] = {".git",".gitignore",".","..","a.out"};

    searchStats stats = initSearchStats();
    
    struct searchIndex index;
    index.size = 0;
    
    recursive(ROOTPATH, &index, &stats);         // searchIndex populated when this finishes
    
    printIndex(&index);
    
    parseIndex(&index, &options);
    
    printStats(&stats);
    
    return 0;
}

void addToIndex(struct searchIndex* si, const char* item_path, ino_t serial, fileType type) {
    strcpy(si->items[si->size].path, item_path);                                             
    si->items[si->size].st_ino = serial;
    si->items[si->size].type = type;
    si->size++;
}

void recursive(char *path, struct searchIndex* index, struct searchStats* stats) {
    struct dirent *dp;
    DIR *dir = opendir(path);

    while ((dp = readdir(dir)) != NULL) {
        char item_path[MAX_PATH_SIZE] = "";                           
        char directory_path[MAX_PATH_SIZE] = "";                        
        const char* item_name = dp->d_name;

        if(filterFileName(item_name) == true) {           
            strcat(item_path,path);                                                                  
            strcat(item_path, item_name);
            fileType type = getFileStatus(item_path);

            switch(type) {
                case REGULAR:
                    stats->file_count++;                                                                // update stats
                    addToIndex(index, item_path, dp->d_ino, type);
                    break;
                case DIREC:
                        stats->dir_count++;
                        getItemPath(path, item_name, &directory_path[0]);                             // update stats
                        recursive(directory_path, index, stats);
                    break;
                default:
                    break;
            }
        }
    } 
    closedir(dir);
}


bool filterFileName(const char * item_name) {
    if(strcmp(item_name, "a.out") != 0 && strcmp(item_name, ".git") != 0 && strcmp(item_name, ".gitignore") != 0 && strcmp(item_name, ".") != 0 && strcmp(item_name, "..") != 0) {
        return true;
    }
    return false;
}

fileType getFileType (mode_t m) {           // @param m | st_mode attribute of struct stat
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
        fileType = getFileType(buffer->st_mode);
    }
    
    return fileType;
}
 
void createSearchItem(struct searchItem* item, ino_t serial, char* path, fileType type) {
    strcpy(item->path, path);
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
 **/
void getItemPath(const char* path, const char* item_name, char* directory_path) {                   
    strcat(directory_path,path);                                                                        
    strcat(directory_path, item_name); 
    strcat(directory_path, "/");                                                         
}
 
void parseFile(operation op, struct searchIndex* index, struct options options, uint64_t size) {
    for (uint64_t i = 0; i < size; i++) {
        op(&(index->items[i]), options);
    }
}

void parseIndex(struct searchIndex* index, struct options* options) {
    switch(options->function) {
        case SEARCH:
            printf("SEARCH! size %llu\n", index->size);
            parseFile(_search,index,*options,index->size);
            break;
        case REPLACE:
            printf("REPLACE!\n");
            parseFile(_replace,index,*options,index->size);
            break;
        case INFO:
            printf("INFO!\n");
            parseFile(_info,index,*options,index->size);
        default:
            break;
    }
}

// ############## helper #################
void printIndex(struct searchIndex* index) {
    for(uint64_t k = 0; k < index->size; k++) {
        //printf("RES: %llu > %s > %llu \n", k, index->items[k].path, index->items[k].st_ino);
    }
}

void printStats(struct searchStats* stats) {
    printf("\nstats:\nf: %llu\nd: %llu\ns: %llu\na: %llu\n", stats->file_count, stats->dir_count, stats->search_count, stats->alter_count);
}
// ######################################## 