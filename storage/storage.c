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
#include <stdlib.h>
#include <direct.h>   // Windows mkdir
#include "storage.h"

#define GREEN   "\033[92m"
#define RED     "\033[91m"
#define YELLOW  "\033[93m"
#define RESET   "\033[0m"

// ============== ENSURE DATA DIRECTORY EXISTS ====================
static void ensure_data_dir() {
    _mkdir("data");
}
// ============== ID GENERATOR FUNCTION ====================
int get_next_id(const char *table_name) {
    char file_name[64];
    sprintf(file_name, "data/%s.db", table_name);

    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        return 1;
    }

    char line[256];
    int last_id = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *id_str = strtok(line, " ");
        if (id_str) {
            int id = atoi(id_str);
            if (id > last_id) last_id = id;
        }
    }

    fclose(fp);
    return last_id + 1;
}

// ------------------------ insert ----------------------------
void storage_insert(Command cmd)
{
    ensure_data_dir(); // <-- ADDED: create data/ if it doesn't exist

    char file_name[64];
    sprintf(file_name, "data/%s.db", cmd.table);

    FILE *fp = fopen(file_name, "a");
    if (!fp) {
        printf(RED "[STORAGE] Error opening file for table '%s'\n" RESET, cmd.table);
        return;
    }

    int id = get_next_id(cmd.table);
    fprintf(fp, "%d %s\n", id, cmd.data);

    fclose(fp);
    printf(GREEN "[STORAGE] Inserted into table '%s' with ID %d\n" RESET, cmd.table, id);
}

// ------------------------ select ----------------------------
void storage_select(Command cmd)
{
    char file_name[64];
    sprintf(file_name, "data/%s.db", cmd.table);

    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        printf(RED "[STORAGE] Table '%s' not found\n" RESET, cmd.table);
        return;
    }

    char line[256];
    int found = 0;
    int total_rows = 0;  // <-- ADDED

    printf(GREEN "[STORAGE] Records from '%s':\n" RESET, cmd.table);

    while (fgets(line, sizeof(line), fp))
    {
        total_rows++;  // <-- ADDED: count every row regardless of WHERE

        if (!cmd.has_where) {
            printf("%s", line);
            found = 1;
            continue;
        }

        char row_copy[256];
        strcpy(row_copy, line);

        char *value = strtok(row_copy, " ");
        if (value && strcmp(value, cmd.where_value) == 0) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        if (total_rows == 0)
            printf(YELLOW "[STORAGE] Table '%s' is empty\n" RESET, cmd.table);  // <-- ADDED
        else
            printf(RED "[STORAGE] No matching records found\n" RESET);
    }

    fclose(fp);
}

// ----------------- delete [row-specific] -------------------------
void storage_delete(Command cmd)
{
    if (!cmd.has_where) {
        printf(RED "[STORAGE] For DELETE: WHERE condition is required.\n" RESET);
        return;
    }

    char file_name[64], temp_name[64];
    sprintf(file_name, "data/%s.db", cmd.table);
    sprintf(temp_name, "data/%s_temp.db", cmd.table);

    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        printf(RED "[STORAGE] Table '%s' not found\n" RESET, cmd.table);
        return;
    }

    FILE *temp_fp = fopen(temp_name, "w");
    if (!temp_fp) {
        printf(RED "[STORAGE] Temp file error\n" RESET);
        fclose(fp);
        return;
    }

    char line[256];
    int deleted_count = 0;

    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = 0;

        char row_copy[256];
        strcpy(row_copy, line);

        char *field_token = strtok(row_copy, " ");
        int match = 0;

        if (field_token != NULL && strcasecmp(cmd.where_column, "id") == 0) {
            if (strcmp(field_token, cmd.where_value) == 0) {
                match = 1;
            }
        }

        if (!match) {
            fprintf(temp_fp, "%s\n", line);
        } else {
            deleted_count++;
        }
    }

    fclose(fp);
    fclose(temp_fp);

    remove(file_name);
    rename(temp_name, file_name);

    printf(GREEN "[STORAGE] Deleted %d rows from table '%s'\n" RESET, deleted_count, cmd.table);
}