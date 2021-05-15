#include <sys/stat.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef FS_H
#define FS_H

#define ROOTPATH "./"          // path where the file-search starts from

#define R "./testDirectory"

#define MAX_PATH_SIZE 1028      // in characters
#define MAX_SEARCH_ITEMS 150   // set to something smart, like OPEN_MAX
#define MAX_DIRECTORIES 50              // TODO: delete if recursive works


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

typedef struct searchItem {
    ino_t st_ino;            // File serial number defined in <sys/stat.h>
    char* path;              // 
    fileType type;           // enum defined above 
    bool success;            // initialize to false 
    char* res_preview;
} searchItem;

typedef struct searchIndex {
    searchItem* items;
    uint64_t size;
} searchIndex;

typedef struct searchStats {
    uint64_t f_count;            // number of files searched 
    uint64_t d_count;            // number of directories searched
} searchStats;


fileType FileType(mode_t m);
fileType getFileStatus (const char* path);
//int parseDirectory(const char* path ,struct searchIndex* index, struct dirIndex* d_index);

//int parseDirectory2(const char* path ,struct searchIndex* index, struct dirIndex* d_index);

int addToSearchIndex(struct searchItem item, struct searchIndex* index);

void getItemPath(const char* path, const char* item_name, char* item_path,  fileType type);
struct searchStats createSearchStats();
struct searchIndex createSearchIndex(struct searchItem* item);
struct searchItem createSearchItem(ino_t serial, char* path, fileType type);

void recursive(char *basePath, struct searchIndex* index);

#endif
