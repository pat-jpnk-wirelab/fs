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

    recursive(ROOTPATH,&index);
    
    printf("EX: %s \n", index.items[2].path);

    return 0;
}


void recursive(char *path, struct searchIndex* index) {
    struct dirent *dp;
    DIR *dir = opendir(path);

    while ((dp = readdir(dir)) != NULL) {

        char item_path[MAX_PATH_SIZE] = "";
        char item_path2[MAX_PATH_SIZE] = "";
        const char* item_name = dp->d_name;

        // filter out standard directories
        if(strcmp(item_name, ".") != 0 && strcmp(item_name, "..") != 0) {
            strcat(item_path,path);                                                                  
            strcat(item_path, item_name);
        
            fileType type = getFileStatus(item_path);
            getItemPath(path, item_name, &item_path2[0], type);
        
            switch(type) {
                case REGULAR:
                    ;
                    struct searchItem item = createSearchItem(dp->d_ino,item_path,type);
                    index->items[2] = item;
                    index->size++;
                    printf("path %llu \n", index->size);
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
struct searchItem createSearchItem(ino_t serial, char* path, fileType type) {               
    struct searchItem item;
    item.path = path;
    item.success = false;    // default false at this point, no search attempted thus far
    item.st_ino = serial;
    item.type = type;

    return item;
}


/**
 * @param void
 * @return struct searchStats - new struct to hold search statistics
**/
struct searchStats createSearchStats() {
    struct searchStats stats;
    stats.file_count = 0;
    stats.dir_count = 0;
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

/**
void parseFile(struct searchIndex* index, struct options search_term, void (*func)(struct searchItem* item, struct options options)) {

} **/

void parseIndex(struct searchIndex* index, enum operation operation, struct options options) {
    uint64_t len = index->size;
    uint64_t i = 0;

    printf("ONE\n");
    switch(operation) {
        case SEARCH:
            printf("TWO %llu\n", len);
            for(i; i < len; i++) {
                
                //printf("ID: %s \n",index->items[i].path);
                //_search(&index->items[i], options);
            }
            break;
        case REPLACE:
            for(i; i < len; i++) {
                _replace(&index->items[i], options);
            }
            break;
    }
}



void dummy(int i) {
    printf("FILE PARSED!!\n");
}




// if operation: SEARCH -> _search function = options[bool capitalization;, bool spacing, char* search_term;]


void _search(struct searchItem* item, struct options options) {

    
    printf("HI");

    //const char* filename = item->path;

    printf("%s \n", item->path);

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
}


// if operation: REPLACE -> _replace function = options[bool capitalization;, bool spacing, char* search_term; char* replacement_term]


void _replace(struct searchItem* item, struct options options) {
    return;
}