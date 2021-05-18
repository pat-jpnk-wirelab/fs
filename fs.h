#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef FS_H
#define FS_H

#define ROOTPATH "./"             
#define MAX_PATH_SIZE 1024                  // in characters
#define MAX_SEARCH_ITEMS 150                // set to something smart, like OPEN_MAX
#define MAX_PREVIEW_SIZE 512                // ????

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

typedef enum func {
    SEARCH,
    REPLACE
} func;

typedef struct options {
    bool capitalization;
    bool spacing;
    func function;
    bool replace;
    char* search_term;
    char* replacement_term;
} options;

typedef struct searchItem {
    ino_t st_ino;                           // File serial number defined in <sys/stat.h>
    char path[MAX_PATH_SIZE];               // changed from char*
    fileType type;                          // enum defined above 
    bool success;                           // initialize to false - success for search 
    bool altered;                           // initilaize to false 
    char res_preview[MAX_PREVIEW_SIZE];     // holds preview if search successful
} searchItem;

typedef struct searchIndex {
    struct searchItem items[MAX_SEARCH_ITEMS];
    uint64_t size;
} searchIndex;

typedef struct searchStats {
    uint64_t file_count;                    // number of files searched 
    uint64_t dir_count;                     // number of directories searched
    uint64_t search_count;                  // number of successful searches
    uint64_t alter_count;                   // number of files altered 
} searchStats;

typedef void (*operation) (struct searchItem*, struct options);         // function pointer parameter accepted by parseFile(...)

struct searchStats initSearchStats();
fileType getFileType(mode_t m);
fileType getFileStatus(const char* path);
void parseFile(operation op, struct searchIndex* index, struct options options, uint64_t size);
void parseIndex(struct searchIndex* index, struct options* options);
void printIndex(struct searchIndex* index);
void getItemPath(const char* path, const char* item_name, char* item_path,  fileType type);
void recursive(char *basePath, struct searchIndex* index, struct searchStats* stats);
void createSearchItem(struct searchItem* item, ino_t serial, char* path, fileType type);
void printStats(struct searchStats* stats);

#endif