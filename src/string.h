#ifndef STRING_H
#define STRING_H

#include "types.h"

size_t strlen(const char* str);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void itoa32(uint32_t value, char* buffer, int base);

#endif