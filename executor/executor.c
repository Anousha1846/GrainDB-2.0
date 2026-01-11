/*
====================================================
FILE: executor.c
ROLE: Command Executor (Action Performer)

This file executes commands parsed by parser.c.

WHAT THIS FILE DOES:
1. Receives a Command structure from main.c.
2. Checks the command type (INSERT, SELECT, DELETE).
3. Performs the corresponding action (currently prints debug info).

WHAT THIS FILE DOES NOT DO:
- It does NOT parse raw input.
- It does NOT handle storage or file operations yet.

Think of this file as the ACTION PERFORMER: it carries out instructions.
====================================================
*/

#include <stdio.h>
#include "executor.h"
#include "storage.h"

void execute_command(Command cmd)
{
    switch (cmd.type)
    {
    case CMD_INSERT:
        printf("[EXECUTOR] INSERT Command received\n");
        printf("Raw input: %s\n", cmd.raw_input);
        storage_insert(cmd); // storage
        break;

    case CMD_SELECT:
        printf("[EXECUTOR] SELECT Command received\n");
        printf("Raw input: %s\n", cmd.raw_input);
        storage_select(cmd); // storage
        break;

    case CMD_DELETE:
        printf("[EXECUTOR] DELETE Command received\n");
        printf("Raw input: %s\n", cmd.raw_input);
        storage_delete(cmd); // storage
        break;

    case CMD_MISSING_TABLE:
        printf("[EXECUTOR] Table name is required\n");
        break;

    case CMD_HELP:
        // do nothing, help already printed
        break;

    case CMD_INVALID:
    printf("[EXECUTOR] Invalid command syntax\n");
    break;

default:
    printf("[EXECUTOR] Unknown Command type\n");
    break;
        break;
    }
}