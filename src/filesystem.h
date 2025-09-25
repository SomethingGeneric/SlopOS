#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "types.h"

#define MAX_FILES 32
#define MAX_FILENAME 32
#define MAX_FILE_SIZE 8192

// Simple file entry in memory
typedef struct {
    char name[MAX_FILENAME];
    uint32_t size;
    uint8_t* data;
    bool in_use;
} file_entry_t;

// Simple in-memory filesystem
typedef struct {
    file_entry_t files[MAX_FILES];
    int file_count;
} filesystem_t;

// Filesystem functions
void filesystem_init();
int filesystem_add_file(const char* name, uint8_t* data, uint32_t size);
file_entry_t* filesystem_find_file(const char* name);
uint8_t* filesystem_read_file(const char* name, uint32_t* size);
void filesystem_list_files(void (*print_func)(const char*));

#endif