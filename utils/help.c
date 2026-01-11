#include <stdio.h>
#include <string.h>
#include "../include/help.h"

#define COLOR_PINK  "\033[0;35m" 
#define COLOR_EX    "\033[0;32m" 
#define COLOR_NOTE  "\033[0;33m" 
#define RED "\033[91m"    // Bright Red
#define COLOR_RESET "\033[0m"

static const HelpEntry help_table[] = {
    {"delete", 
     "Remove rows from a table", 
     "DELETE <tableName> WHERE id = <idValue>", 
     "DELETE users WHERE id = 3", 
     "Action is irreversible; use WHERE to avoid data loss. Where clause is mandatory."},

    {"help",
     "Display engine documentation", 
     "HELP <commandName>", 
     "help select", 
     "Shows all commands if no argument is provided."},

    {"insert", "Append a new row to the storage layer", 
     "INSERT <tableName> <values>", 
     "INSERT users Anousha", 
     "Values must match the defined schema data types. ID is auto-generated."},

    {"select", "Query and retrieve data from tables", 
     "SELECT <tableName> WHERE id = <idValue>   \n \t  OR SELECT <tableName>", 
     "SELECT users WHERE id = 10   \n \t  OR SELECT users", 
     "Supports both, full table scans and where clause."}
};

static const size_t HELP_COUNT = sizeof(help_table) / sizeof(help_table[0]);

void show_general_help() {
    printf("\n"COLOR_PINK "  GRAIN_DB    | Supported Commands" COLOR_RESET "\n");
    printf("  --------------------------------------------\n");
    for (size_t i = 0; i < HELP_COUNT; i++) {
        printf("  " COLOR_PINK "%-10s" COLOR_RESET " %s\n", 
               help_table[i].name, help_table[i].description);
    }
    printf("\n  Type " COLOR_PINK "help <commandName>" COLOR_RESET " for detailed syntax.\n");
    printf("  NOTE: None of the commands are case-sensitive.\n\n");

}

void show_command_help(const char *command) {
    for (size_t i = 0; i < HELP_COUNT; i++) {
        if (strcasecmp(help_table[i].name, command) == 0) {
            printf(COLOR_PINK "COMMAND:     %s" COLOR_RESET "\n", help_table[i].name);
            printf("DESCRIPTION: %s\n", help_table[i].description);
            printf("SYNTAX:      " COLOR_PINK "%s" COLOR_RESET "\n", help_table[i].syntax);
            printf("EXAMPLE:     " COLOR_EX "%s" COLOR_RESET "\n", help_table[i].example);
            printf("NOTES:       " COLOR_NOTE "%s" COLOR_RESET "\n\n", help_table[i].notes);
            return;
        }
    }
    printf(RED "Unknown command: '%s'. Type 'help' for the list." COLOR_RESET "\n\n", command);
}

void handle_help_command(const char **args, int argc) {
    // If argc is 2, user typed "help select" (args[0] is help, args[1] is select)
    if (argc > 1) {
        show_command_help(args[1]);
    } else {
        show_general_help();
    }
}