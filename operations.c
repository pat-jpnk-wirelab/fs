#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "colors.h"
#include <errno.h>

/**
 * here functions that conform to the defined type 'operation' are defined
 * they are a parameter in the method parseFile(...) defined in fs.c
 * 
 * must have an equivalent in "enum func" in fs.h
 **/


// if operation: SEARCH -> _search function = options[bool capitalization;, bool spacing, char* search_term;]
void _search(struct searchItem* item, struct options options) {
    FILE *fp = fopen(item->path, "r");                               

    if (fp == NULL) {
        fprintf(stderr, "Error opening %s %s\n", item->path, strerror(errno)); // e.g. permission denied
        return;
    }

    char* buffer = NULL;    
    size_t len;
    size_t search_term_len = strlen(options.search_term);
    ssize_t bytes_read = getdelim(&buffer, &len, EOF, fp); 
    ssize_t errval = -1;

    if (bytes_read != errval) {
        // this code wont be reached if file is empty
        //printf("item :%s -- char of buffer: %c >> SIZE: %zu\n",item->path,*(buffer+1), len);
        uint64_t i = (uint64_t) len;
        uint64_t j = 0;
        uint64_t line_count = 1;
        uint64_t j_cache = 0;

        while(j < i) {
            if(*(buffer+j) == '\n') {
                line_count++;
                j_cache = 0;                // for counting position in line in finding / TODO: add better name 
            }
            if(*(buffer+j) == options.search_term[0]) {                           
                bool mismatch = false;
                while(mismatch == false) {  
                    for(size_t k = 1; k < search_term_len; k++) {
                        if((*(buffer+j+k) != options.search_term[k])) {
                            mismatch = true;
                            break;
                        }
                    }
                    if(mismatch == false) {
                        printf(KWHT"%llu:%llu %s\n"KRESET, line_count, j_cache, item->path);                    
                    }
                    break;
                } 
            }
            j_cache++;
            j++;
        }
    } else {
        fprintf(stderr,KNRM"%s not read, file empty\n"KRESET, item->path); // triggers on empty file
    }
    fclose(fp);
}

// opens files in read and write mode
// if operation: REPLACE -> _replace function = options[bool capitalization;, bool spacing, char* search_term; char* replacement_term]
void _replace(struct searchItem* item, struct options options) {
    //printf("Hello world from replace %s %s\n", options.search_term, options.replacement_term);
    FILE *fp = fopen(item->path, "r+"); 

    if (fp == NULL) {
        fprintf(stderr, "Error opening %s %s\n", item->path, strerror(errno)); // e.g. permission denied
        return;
    }
     
    char* buffer = NULL;    
    size_t len;

    size_t search_term_len = strlen(options.search_term);
    size_t replacement_term_len = strlen(options.replacement_term);
    
    ssize_t bytes_read = getdelim(&buffer, &len, EOF, fp); 
    ssize_t errval = -1;

    char new_buffer[len];
    size_t new_buffer_i = 0;

    if (bytes_read != errval) {
        uint64_t i = (uint64_t) len;
        uint64_t j = 0;
        //uint64_t line_count = 1;
        uint64_t j_cache = 0;

        size_t  k_cache = 0;

        bool mismatch;

        while(j < i) {
            if(*(buffer+j) == options.search_term[0]) {                 // buffer+j = first char place                         
                mismatch = false; 

                for(size_t k = 1; k < search_term_len; k++) {           // len l, l = 4 -> 4[0] + 4[1,2,3]          1 to len-1      [  1 to l = 1,   ]
                    if((*(buffer+j+k) != options.search_term[k])) {     // + j + 1,    ...   ,   + j + 3
                        k_cache = k;  
                        mismatch = true;
                        printf("JAVA\n");
                        break;
                    }
                }

                // triggers when full match    
                if(mismatch == false) {    
                    printf("CODE REACHED 2\n");

                    for(size_t i = 0; i < replacement_term_len; i++) {
                        new_buffer[new_buffer_i] = options.replacement_term[0];
                        new_buffer_i++;
                    }
                    j = (j+replacement_term_len)-1;
                }        

                // triggers when [0] matched but rest not fully
                if (mismatch == true) {
                    printf("PARTLY\n");
                    for(size_t t = j; t < (j+k_cache); t++) {
                        new_buffer[new_buffer_i] = *(buffer+t);
                        new_buffer_i++;
                    }
                    j = j+k_cache-1;
                } 
            } else {
                // triggers when != st[0]
                new_buffer[new_buffer_i] = *(buffer+j);
                new_buffer_i++;
            }

            j_cache++;
            j++;
        }
    } else {
        fprintf(stderr,KNRM"%s not read, file empty\n"KRESET, item->path); // triggers on empty file
    }

   // for(size_t i = 0; i < new_buffer_i; i++) {
   //     printf("%c", new_buffer[i]);
   // }
   // printf("\n");
}

// does not open files
void _info(struct searchItem* item, struct options options) {
    printf(KWHT"%llu %s\n"KRESET,item->st_ino, item->path);
}

