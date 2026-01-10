
/*
====================================================
FILE: parser.h
ROLE: Parser Interface Definition

This header file declares the interface of the
parser module.

WHAT THIS FILE DOES:
- Exposes the parse_command() function.
- Allows other modules to use the parser.
- Shares the Command structure via common.h.

WHAT THIS FILE DOES NOT DO:
- It does NOT implement parsing logic.
- It does NOT define shared data structures.
====================================================
*/

#ifndef PARSER_H
#define PARSER_H

#include "common.h"

Command parse_command(const char *input);

#endif
