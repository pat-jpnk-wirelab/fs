#include <stdint.h>
#include "fs.h"

#ifndef OPERATIONS_H
#define OPERATIONS_H

void _search(struct searchItem* item, struct options options);                 // for searching in files
void _replace(struct searchItem* item, struct options options);                // for replacing things in files

#endif