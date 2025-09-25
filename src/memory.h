#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

// Memory layout constants
#define KERNEL_START        0x100000    // 1MB - where kernel is loaded
#define KERNEL_HEAP_START   0x400000    // 4MB - start of kernel heap  
#define USER_SPACE_START    0x800000    // 8MB - start of user space
#define PAGE_SIZE           4096        // 4KB pages
#define PAGES_PER_TABLE     1024        // Pages per page table
#define TABLES_PER_DIR      1024        // Page tables per directory

// Page directory and table entry flags
#define PAGE_PRESENT        0x001
#define PAGE_WRITABLE       0x002
#define PAGE_USER           0x004
#define PAGE_ACCESSED       0x020
#define PAGE_DIRTY          0x040

// Physical memory management
typedef struct {
    uint32_t* bitmap;       // Bitmap of free/used pages
    uint32_t total_pages;   // Total number of pages
    uint32_t used_pages;    // Number of used pages
    uint32_t start_addr;    // Start of managed memory
} physical_memory_t;

// Virtual memory structures
typedef struct {
    uint32_t tables[TABLES_PER_DIR];
} page_directory_t;

typedef struct {
    uint32_t pages[PAGES_PER_TABLE];
} page_table_t;

// Memory management functions
void memory_init(uint32_t mem_size);
void* kmalloc(size_t size);
void kfree(void* ptr);

// Memory statistics
typedef struct {
    uint32_t total_pages;
    uint32_t used_pages;
    uint32_t free_pages;
    uint32_t heap_used;
} memory_stats_t;

void memory_get_stats(memory_stats_t* stats);

// Physical memory management
uint32_t pmm_alloc_page();
void pmm_free_page(uint32_t page_addr);
void pmm_init(uint32_t mem_size);

// Virtual memory management
void vmm_init();
page_directory_t* vmm_create_page_directory();
void vmm_map_page(page_directory_t* dir, uint32_t virt_addr, uint32_t phys_addr, uint32_t flags);
void vmm_switch_directory(page_directory_t* dir);

#endif