/*
====================================================
FILE: executor.h
ROLE: Executor Header (Function Declaration)

This file declares the interface for executing commands.
// Executes a parsed command by dispatching it to the appropriate subsystem (storage, etc.)

WHAT THIS FILE DOES:
1. Includes necessary definitions from common.h.
2. Declares the function execute_command(Command cmd) for use in main.c or other modules.

WHAT THIS FILE DOES NOT DO:
- It does NOT execute commands itself.
- It does NOT parse input or handle storage.

Think of this file as the CONTRACT for the executor: it tells other files how to request actions.
====================================================
*/


#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "common.h"

void  execute_command(Command cmd);

#endif