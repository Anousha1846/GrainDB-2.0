// storage.c will:
// 1. Open files
// 2. Write records
// 3. Read records
// 4. Close files

// storage will NOT:
// Understand commands
// Decide logic
// Print prompt
// Parse text
// It only obeys orders.

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for atoi
#include "storage.h"
#define GREEN   "\033[92m"    // Bright Green 
#define RED     "\033[91m"    // Bright Red
#define YELLOW  "\033[93m"    // Bright Yellow
#define RESET   "\033[0m"
//============== ID GENETATOR FUNTION ====================
int get_next_id(const char *table_name) {
    char file_name[64];
    sprintf(file_name, "data/%s.db", table_name);

    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        // table not found, first row = 1
        return 1;
    }

    char line[256];
    int last_id = 0;

    while (fgets(line, sizeof(line), fp)) {
        // split by space, first token = ID
        char *id_str = strtok(line, " ");
        if (id_str) {
            int id = atoi(id_str);
            if (id > last_id) last_id = id;
        }
    }

    fclose(fp);
    return last_id + 1; // next ID

}
//------------------------insert----------------------------
void storage_insert(Command cmd)
{
    // Build the filename based on the table name
    char file_name[64];
    sprintf(file_name, "data/%s.db", cmd.table);
    // Open file in append mode (a = add to end)
    FILE *fp = fopen(file_name, "a");
    if (!fp)
    { // error handling
        printf(RED "[STORAGE] Error opening file for table '%s'\n" RESET, cmd.table);
        return;
    }
   // figure out the ID
int id = get_next_id(cmd.table);
fprintf(fp, "%d %s\n", id, cmd.data);


fclose(fp);
printf(GREEN "[STORAGE] Inserted into table '%s' with ID %d\n" RESET, cmd.table, id);

}

//------------------------ select --------------------
void storage_select(Command cmd)
{
    // Build the filename based on the table name
    char file_name[64];
    sprintf(file_name, "data/%s.db", cmd.table); 
    // Open file in read-only mode (r)
    FILE *fp = fopen(file_name, "r");
    
    if (!fp)
    { // error handling
        printf(RED "[STORAGE] Table '%s' not found\n" RESET, cmd.table);
        return;
    }
    // Buffer to store each line while reading
    char line[256];
    int found = 0;
    printf(GREEN "[STORAGE] Records from '%s':\n" RESET, cmd.table);
    while (fgets(line, sizeof(line), fp))
    {
        if (!cmd.has_where) // No WHERE → print everything
        {
           printf("%s", line);
           found = 1;
           continue;
        }
        // WHERE exists → check column 0 (ID)
        char row_copy[256];
        strcpy(row_copy, line);

        char *value = strtok(row_copy, " ");

        if (value && strcmp(value,cmd.where_value)==0)
        {
           printf("%s", line);
           found = 1;
        }
    }
    if (!found)
    {
        printf(RED "[STORAGE] No matching records found\n" RESET);
    }
    
    fclose(fp); // Close the file to save changes
}

//----------------- delete [ row-specific]-------------------------
void storage_delete(Command cmd)
{
     // Check if WHERE condition exists
    if (!cmd.has_where)
    {
        printf(RED "[STORAGE] For DELETE : WHERE condition is required.\n" RESET);
        return;
    }
    // Build the filename based on the table name
    char file_name[64], temp_name[64];
    sprintf(file_name, "data/%s.db", cmd.table);
    sprintf(temp_name, "data/%s_temp.db", cmd.table);

    FILE *fp = fopen(file_name, "r");
    if (!fp)
    {
        printf(RED "[STORAGE] Table '%s' not found\n" RESET, cmd.table);
        return;
    }
    FILE *temp_fp = fopen(temp_name, "w");
    if (!temp_fp)
    {
        printf(RED "[STORAGE] Temp file error\n" RESET);
        fclose(fp);
        return;
    }
    //process rows
    char line[256];
    int deleted_count = 0;

    while (fgets(line, sizeof(line), fp))
    {
       line[strcspn(line, "\n")] = 0; //remove newline

       // split row into fields (assuming space-separated)
        char row_copy[256];
        strcpy(row_copy, line);

        char *field_token = strtok(row_copy, " "); //first field=id
        int match = 0;

        if (field_token != NULL && strcasecmp(cmd.where_column, "id")==0)
        {
            
           if (strcmp(field_token, cmd.where_value)==0)
           {
            match = 1; //ye row ho jaegi delete, insha allah
           }
        }
        if (!match)
        {
            fprintf(temp_fp, "%s\n", line); //row is kept heree
        }else{
            deleted_count++;
        }
    }
    
    fclose(fp);
    fclose(temp_fp);

    // Replace original file with temp file
    remove(file_name);
    rename(temp_name, file_name);

    printf(GREEN "[STORAGE] Deleted %d rows from table '%s'\n" RESET, deleted_count, cmd.table);
}
