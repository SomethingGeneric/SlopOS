#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

void terminal_initialize();
void terminal_putchar(char c);
void terminal_writestring(const char* data);
char terminal_getchar();
void terminal_getstring(char* buffer, size_t max_length);
void terminal_update_cursor();

#endif