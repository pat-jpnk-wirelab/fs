#include "operations.h"
#include <stdio.h>
#include <string.h>

/**
 * here functions that conform to the defined type 'operation' are defined
 * they are a parameter in the method parseFile(...) defined in fs.c
 * 
 * must have an equivalent in "enum func" in fs.h
 **/

// opens files in read only mode
// if operation: SEARCH -> _search function = options[bool capitalization;, bool spacing, char* search_term;]
void _search(struct searchItem* item, struct options options) {
    FILE *fp = fopen(item->path, "r");
    char c;

    size_t search_term_len = strlen(options.search_term);

    printf("PATH: %s ST: %s L: %zu\n", item->path, options.search_term, search_term_len);
    
    char* buffer = NULL;
    size_t len;
    ssize_t bytes_read = getdelim( &buffer, &len, EOF, fp);
    if (bytes_read != -1) {
        // this code wont be reached if file is empty
        //printf("item :%s -- char of buffer: %c >> SIZE: %zu\n",item->path,*(buffer+1), len);

        unsigned int i = (int) len;
        unsigned int j = 0;

        unsigned int line_count = 1;
        unsigned int j_cache = j;

        while(j < i) {
            
            if(*(buffer+j) == '\n') {
                line_count++;
                j_cache = 0;
            }

            if((*(buffer+j) == options.search_term[0])) {
                bool mismatch = false;

                while(mismatch == false) {  
                    for(size_t k = 1; k < search_term_len; k++) {
                        if((*(buffer+j+k) != options.search_term[k])) {
                            mismatch = true;
                            break;
                        }
                    }
                    if(mismatch == false) {
                        printf("\t\tMATCH FOUND; POS: %u IN %s LINEL: %u\n", j_cache, item->path, line_count);
                        printf("LINE %c\n", *(buffer+11));
                    }
                    break;
                } 
            }
            j_cache++;
            j++;
        }
    } else {
        printf("Error");
    }
    fclose(fp);
    
}

// opens files in read and write mode
// if operation: REPLACE -> _replace function = options[bool capitalization;, bool spacing, char* search_term; char* replacement_term]
void _replace(struct searchItem* item, struct options options) {
    printf("Hello world from replace\n");
}


void _info(struct searchItem* item, struct options options) {
    printf("Hello world from replace\n");
}

