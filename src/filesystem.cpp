#include "filesystem.h"
#include "memory.h"
#include "string.h"

static filesystem_t fs;

// Initialize the filesystem
void filesystem_init() {
    fs.file_count = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        fs.files[i].in_use = false;
        fs.files[i].data = NULL;
        fs.files[i].size = 0;
        fs.files[i].name[0] = '\0';
    }
}

// Add a file to the filesystem
int filesystem_add_file(const char* name, uint8_t* data, uint32_t size) {
    if (fs.file_count >= MAX_FILES || size > MAX_FILE_SIZE) {
        return -1; // No space or file too large
    }
    
    // Find free slot
    int slot = -1;
    for (int i = 0; i < MAX_FILES; i++) {
        if (!fs.files[i].in_use) {
            slot = i;
            break;
        }
    }
    
    if (slot == -1) {
        return -1; // No free slot
    }
    
    file_entry_t* file = &fs.files[slot];
    
    // Copy name
    int name_len = strlen(name);
    if (name_len >= MAX_FILENAME) {
        name_len = MAX_FILENAME - 1;
    }
    strncpy(file->name, name, name_len);
    file->name[name_len] = '\0';
    
    // Allocate memory for file data and copy it
    file->data = (uint8_t*)kmalloc(size);
    if (!file->data) {
        return -1; // Memory allocation failed
    }
    
    memcpy(file->data, data, size);
    file->size = size;
    file->in_use = true;
    fs.file_count++;
    
    return slot;
}

// Find a file by name
file_entry_t* filesystem_find_file(const char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs.files[i].in_use && strcmp(fs.files[i].name, name) == 0) {
            return &fs.files[i];
        }
    }
    return NULL;
}

// Read file data
uint8_t* filesystem_read_file(const char* name, uint32_t* size) {
    file_entry_t* file = filesystem_find_file(name);
    if (!file) {
        if (size) *size = 0;
        return NULL;
    }
    
    if (size) *size = file->size;
    return file->data;
}

// List all files
void filesystem_list_files(void (*print_func)(const char*)) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs.files[i].in_use) {
            print_func(fs.files[i].name);
            print_func("\n");
        }
    }
}