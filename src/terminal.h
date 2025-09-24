#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

void terminal_initialize();
void terminal_putchar(char c);
void terminal_writestring(const char* data);
char terminal_getchar();

#endif