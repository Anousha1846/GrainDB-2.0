/*
====================================================
FILE: main.c
ROLE: Program Entry Point (Reception Desk)

This file is responsible for handling all interaction between the user and the database engine.

WHAT THIS FILE DOES:
1. Starts the database engine and prints a welcome message.
2. Displays a command prompt (Anousha_db>).
3. Reads user input safely using fgets.
4. Handles the 'exit' command directly.
5. Sends user input to the parser module.
6. Receives a parsed Command structure.
7. Forwards valid commands to the executor module.
8. Keeps the program running in a loop until exit.

WHAT THIS FILE DOES NOT DO:
- It does NOT understand command syntax.
- It does NOT execute database logic.
- It does NOT read or write any files.

Think of this file as the RECEPTION DESK:
It listens, forwards requests, and shows responses,but never makes decisions itself.
====================================================
*/

// Text color macros
#define CYAN    "\033[1;36m"   // Bold Cyan - for prompts
#define GREEN   "\033[1;32m"   // Bold Green - for success messages
#define RED     "\033[1;31m"   // Bold Red - for errors
#define YELLOW  "\033[1;33m"   // Bold Yellow - for warnings
#define RESET   "\033[0m"      // Reset color to default

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "executor.h"
#include "parser.h"

#define MAX_INPUT 1024
int main(){
    char input[MAX_INPUT];
    printf("\nWelcome to GRAIN_DB!\n");
    printf("Type 'exit' to quit.\n");

    while (1)
    {
        printf(CYAN "Grain_db> " RESET);
        if(fgets(input, sizeof(input), stdin)==NULL){
            printf("\n");
            break;
        }
        //newline remove
        input[strcspn(input, "\n")] ='\0';

        //ext command
        if(strcasecmp(input, "exit")==0){
            printf("Exiting GRAIN_DB.\n");
            break;
        }
        //parse command
        Command cmd = parse_command(input);
        if(cmd.type == CMD_INVALID){
            printf(RED "Invalid Command\n" RESET);
            continue;
        }

        //execute command
        execute_command(cmd);
    }
    return 0;    

}

