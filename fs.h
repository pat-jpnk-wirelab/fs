#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef FS_H
#define FS_H

#define ROOTPATH "./"             

#define MAX_PATH_SIZE 1028              // in characters
#define MAX_SEARCH_ITEMS 150            // set to something smart, like OPEN_MAX


typedef enum fileType {
    SOCKET,
    SYMLINK,                // how to handle ?
    REGULAR,                // Search 
    BLOCK,                  // ?
    DIREC,                      
    CHAR,                   // ?
    PIPE,         
    UNKNOWN                 // ignore for now 
} fileType;

typedef enum operation {
    SEARCH,
    REPLACE
} operation;

typedef struct options {
    bool capitalization;
    bool spacing;
    char* search_term;
    char* replacement_term;
} options;

typedef struct searchItem {
    ino_t st_ino;            // File serial number defined in <sys/stat.h>
    char* path;                                                   // changed from char*
    fileType type;           // enum defined above 
    bool success;            // initialize to false - success for search 
    bool altered;            // initilaize to false 
    char* res_preview;
} searchItem;

typedef struct searchIndex {
    searchItem items[MAX_SEARCH_ITEMS];
    uint64_t size;
} searchIndex;

typedef struct searchStats {
    uint64_t file_count;             // number of files searched 
    uint64_t dir_count;              // number of directories searched
    uint64_t search_count;           // number of successful searches
    uint64_t alter_count;            // number of files altered 
} searchStats;



void getItemPath(const char* path, const char* item_name, char* item_path,  fileType type);
void recursive(char *basePath, struct searchIndex* index);

struct searchStats createSearchStats();
struct searchItem createSearchItem(ino_t serial, char* path, fileType type);

fileType FileType(mode_t m);
fileType getFileStatus(const char* path);

void parseFile(const char *filename, char* search_term, void (*func)());

// search / replace = (both) => (
//    capitalize matters: yes/no                     (default: exact capitalization) 
//    spaces (term must stand seprarated): yes/no    (default: yes)          
//  ) 
void parseIndex(struct searchIndex* index, enum operation operation, struct options options);

void dummy(int i);

void _search(struct searchItem* item, struct options options);
void _replace(struct searchItem* item, struct options options);

#endif




