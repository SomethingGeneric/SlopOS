#include "memory.h"
#include "string.h"

// Global memory management structures
static physical_memory_t pmm;
static page_directory_t* kernel_directory;
static uint32_t kernel_heap_pos = KERNEL_HEAP_START;

// Simple bitmap operations
static void bitmap_set_bit(uint32_t* bitmap, uint32_t bit) {
    bitmap[bit / 32] |= (1 << (bit % 32));
}

static void bitmap_clear_bit(uint32_t* bitmap, uint32_t bit) {
    bitmap[bit / 32] &= ~(1 << (bit % 32));
}

static int bitmap_test_bit(uint32_t* bitmap, uint32_t bit) {
    return bitmap[bit / 32] & (1 << (bit % 32));
}

// Physical Memory Manager initialization
void pmm_init(uint32_t mem_size) {
    // Calculate number of pages
    pmm.total_pages = mem_size / PAGE_SIZE;
    pmm.used_pages = 0;
    pmm.start_addr = KERNEL_START;
    
    // Allocate bitmap at end of kernel (simple placement)
    uint32_t bitmap_size = (pmm.total_pages + 31) / 32 * sizeof(uint32_t);
    pmm.bitmap = (uint32_t*)kernel_heap_pos;
    kernel_heap_pos += bitmap_size;
    
    // Clear bitmap (all pages free initially)
    for (uint32_t i = 0; i < (pmm.total_pages + 31) / 32; i++) {
        pmm.bitmap[i] = 0;
    }
    
    // Mark kernel pages as used (0-4MB)
    uint32_t kernel_pages = KERNEL_HEAP_START / PAGE_SIZE;
    for (uint32_t i = 0; i < kernel_pages; i++) {
        bitmap_set_bit(pmm.bitmap, i);
        pmm.used_pages++;
    }
}

// Allocate a physical page
uint32_t pmm_alloc_page() {
    for (uint32_t i = 0; i < pmm.total_pages; i++) {
        if (!bitmap_test_bit(pmm.bitmap, i)) {
            bitmap_set_bit(pmm.bitmap, i);
            pmm.used_pages++;
            return i * PAGE_SIZE;
        }
    }
    return 0; // Out of memory
}

// Free a physical page
void pmm_free_page(uint32_t page_addr) {
    uint32_t page = page_addr / PAGE_SIZE;
    if (page < pmm.total_pages && bitmap_test_bit(pmm.bitmap, page)) {
        bitmap_clear_bit(pmm.bitmap, page);
        pmm.used_pages--;
    }
}

// Virtual Memory Manager initialization
void vmm_init() {
    // For simplicity, skip paging setup for now
    // Just use identity mapping (virtual = physical)
    kernel_directory = NULL;
}

// Create a new page directory
page_directory_t* vmm_create_page_directory() {
    // For now, just return NULL since we're not using paging
    return NULL;
}

// Map a virtual address to a physical address
void vmm_map_page(page_directory_t* dir, uint32_t virt_addr, uint32_t phys_addr, uint32_t flags) {
    // Skip paging for now - identity mapping
    (void)dir; (void)virt_addr; (void)phys_addr; (void)flags;
}

// Switch to a different page directory
void vmm_switch_directory(page_directory_t* dir) {
    // Skip paging for now
    (void)dir;
}

// Simple kernel heap allocator
void* kmalloc(size_t size) {
    // Align to 4-byte boundary
    size = (size + 3) & ~3;
    
    if (kernel_heap_pos + size >= USER_SPACE_START) {
        return NULL; // Out of kernel heap space
    }
    
    void* ptr = (void*)kernel_heap_pos;
    kernel_heap_pos += size;
    return ptr;
}

// Simple free (for now, just a stub - no actual deallocation)
void kfree(void* ptr) {
    // For this simple implementation, we don't actually free memory
    // A real implementation would maintain a free list
    (void)ptr; // Suppress unused parameter warning
}

// Main memory initialization
void memory_init(uint32_t mem_size) {
    // Initialize physical memory manager
    pmm_init(mem_size);
    
    // Initialize virtual memory manager  
    vmm_init();
}