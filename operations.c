#include "operations.h"
#include <stdio.h>

/**
 * here functions that conform to the defined type 'operation' are defined
 * they are a parameter in the method parseFile(...) defined in fs.c
 * 
 * must have an equivalent in "enum func" in fs.h
 **/

// if operation: SEARCH -> _search function = options[bool capitalization;, bool spacing, char* search_term;]
void _search(struct searchItem* item, struct options options) {
    printf("Hello World from search\n");

    // print contents of file

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
    printf("Hello world from replace\n");
}