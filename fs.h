#include <sys/stat.h>
#include <stdbool.h>

#ifndef FS_H
#define FS_H

#define ROOTPATH "./"        // path where the file-search starts from


#define MAX_SEARCH_ITEMS 150   // set to something smart, like OPEN_MAX


typedef enum fileType {
    SOCKET,
    SYMLINK,
    REGULAR,
    BLOCK,
    DIREC, 
    CHAR,
    PIPE,
    UNKNOWN
} fileType;

typedef struct searchIndex {
    ino_t st_ino;            // File serial number defined in <sys/stat.h>
    char* path;
    fileType type;
    bool success;
    char* res_preview;
} searchItem;

typedef struct searchStats {
    int64_t f_count;            // number of files searched 
    int64_t d_count;            // number of directories searched
} searchStats;


fileType FileType(mode_t m);

int getFileStatus (const char* path);






#endif
