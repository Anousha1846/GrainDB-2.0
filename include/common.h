/*
====================================================
FILE: common.h
ROLE: Shared Definitions (Common Language)

This file contains definitions that are shared
across multiple modules of the database engine.

WHAT THIS FILE DEFINES:
- MAX_INPUT: Maximum size of user input.
- CommandType: Enum representing different command types.
- Command: Structure used to pass parsed commands between modules.


WHY THIS FILE EXISTS:
Multiple modules (main, parser, executor, storage)
need to understand the same Command format.
This file ensures consistency across the system.

RULE:
Any data structure used by more than one module
must be defined here.
====================================================
*/

#ifndef COMMON_H
#define COMMON_H

#define MAX_INPUT 1024

typedef enum
{
    CMD_INVALID,
    CMD_INSERT,
    CMD_SELECT,
    CMD_DELETE,
    CMD_EXIT,
    CMD_MISSING_TABLE,  
    CMD_HELP,
} CommandType;

typedef struct
{
    CommandType type;
    char raw_input[MAX_INPUT];
    char table[32]; // M1 table structure
    char data[256]; // M2 row structure
    int has_where; //M3 implemeting where
    char where_column[32];//M3
    char where_value[32];//M3
} Command;

#endif
