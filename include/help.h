#ifndef HELP_H
#define HELP_H

//entry structure
typedef struct {
    const char *name;
    const char *description;
    const char *syntax;
    const char *example;
    const char *notes;
}HelpEntry;

void show_general_help();
void show_command_help(const char *command);
void handle_help_command(const char **args, int argc);

#endif // HELP_H
