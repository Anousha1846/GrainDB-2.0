
#ifndef STORAGE_H
#define STORAGE_H

#include "common.h"

// store a records from a table file
void storage_insert(Command cmd);

// read all records from a table file
void storage_select(Command cmd);

// remove records
void storage_delete(Command cmd);


#endif

