/*
====================================================
FILE: parser.c
ROLE: Command Parser (Translator)

This file is responsible for understanding what the user is trying to do, based on raw text input.

WHAT THIS FILE DOES:
1. Receives a raw input string from main.c.
2. Analyzes the first keyword of the command.
3. Determines the command type (INSERT, SELECT, DELETE).
4. Stores the original input inside a Command structure.
5. Returns a filled Command structure to main.c.

WHAT THIS FILE DOES NOT DO:
- It does NOT execute commands.
- It does NOT access files.
- It does NOT modify stored data.

Think of this file as a TRANSLATOR:
It reads a sentence and tells the system what it means, without taking any action.
====================================================
*/

#include <stdio.h>
#include <string.h>
#include "parser.h"
#define GREEN   "\033[92m"    // Bright Green 
#define RED     "\033[91m"    // Bright Red
#define YELLOW  "\033[93m"    // Bright Yellow
#define RESET   "\033[0m"
//==========================
void normalize_expression(char *expr)
{
    char buffer[64];
    int j = 0;

    for (int i = 0; expr[i]; i++)
    {
        if (expr[i] != ' ')
            buffer[j++] = expr[i];
    }
    buffer[j] = '\0';
    strcpy(expr, buffer);
}
//====================================================
Command parse_command(const char *input)
{
    Command cmd;
    memset(&cmd, 0, sizeof(Command));
    strcpy(cmd.raw_input, input); // stores raw input

    // make a mutable copy of input for strtok
    char input_copy[256];
    strcpy(input_copy, input);

    // first token = command keyword
    char *token = strtok(input_copy, " "); // split by spaces

    if (token == NULL) // if no cmnd entered after insert
    {
        cmd.type = CMD_INVALID;
        return cmd;
    }
    // ----------------------- INSERT ------------------------
    if (strcasecmp(token, "insert") == 0)
    {
        cmd.type = CMD_INSERT;
        token = strtok(NULL, " "); // next token = table name
        if (!token)
        {
            cmd.type = CMD_MISSING_TABLE;
            return cmd;
        }
        strcpy(cmd.table, token); // store table name
        // Everything after table name = row data
        char *data_start = strstr(input, cmd.table);
        data_start += strlen(cmd.table); // move past table name

        while (*data_start == ' ')
        {
            data_start++;
        }
        strcpy(cmd.data, data_start);
        // if not data is input after table formation
        if (strlen(cmd.data) == 0)
        {
            printf(RED "[PARSER] Row data is required for INSERT\n" RESET);
            // cmd.type = CMD_INVALID; // prevent krega execution
            return cmd;
        }
    }
    // ----------------------- SELECT ------------------------
    else if (strcasecmp(token, "select") == 0)
    {
        cmd.type = CMD_SELECT;
        token = strtok(NULL, " "); // next word = table name
        if (!token)
        {
            cmd.type = CMD_MISSING_TABLE;
            return cmd;
        }
        strcpy(cmd.table, token);
        // check for where
        token = strtok(NULL, " "); // maybe "where"
        if (token && strcasecmp(token, "where") == 0)
        {
            cmd.has_where = 1;
            token = strtok(NULL, " "); // id OR id=1
            char *expression = token;
            // If '=' is not in this token, skip '=' and read value
            if (expression && !strchr(expression, '='))
            {
                strtok(NULL, " ");         // skip '='
                token = strtok(NULL, " "); // value
                strcat(expression, "=");
                strcat(expression, token);
            }
            normalize_expression(expression);
            sscanf(expression, "%[^=]=%s", cmd.where_column, cmd.where_value);
        }
    }
    // ----------------------- DELETE ------------------------
    else if (strcasecmp(token, "delete") == 0)
    {
        cmd.type = CMD_DELETE;
        token = strtok(NULL, " ");
        if (!token)
        {
            printf(RED "[PARSER] DELETE command requires a table name\n" RESET);
            // cmd.type = CMD_INVALID;
            return cmd;
        }
        strcpy(cmd.table, token); // store table name
        // check for WHERE clause
        token = strtok(NULL, " ");
        if (token && strcasecmp(token, "where") == 0)
        {
            cmd.has_where = 1;
            token = strtok(NULL, " "); // column OR column=value
            if (!token)
            {
                cmd.type = CMD_INVALID;
                return cmd;
            }
            // id=1 (no spaces)
            char *equal_position = strchr(token, '=');
            if (equal_position)
            {
                *equal_position = '\0';
                strcpy(cmd.where_column, token);
                strcpy(cmd.where_value, equal_position + 1);
            }
            else
            {
                // id = 1 (spaces)
                strcpy(cmd.where_column, token);
                strtok(NULL, " ");         // skip '='
                token = strtok(NULL, " "); // value
                if (!token)
                {
                    cmd.type = CMD_INVALID;
                    return cmd;
                }
                strcpy(cmd.where_value, token);
            }
        }
    }
    // ----------------------- INVALID ------------------------
    else
    {
        cmd.type = CMD_INVALID;
    }

    return cmd;
}