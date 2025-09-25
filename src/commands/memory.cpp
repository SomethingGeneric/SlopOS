#include "memory.h"
#include "../memory.h"

// Function to execute memory command from shell
void execute_memory_command(void (*write_string)(const char*)) {
    memory_stats_t stats;
    memory_get_stats(&stats);
    
    write_string("Memory Information:\n");
    
    // Display total pages
    write_string("  Total pages: ");
    char num_str[32];
    int pos = 0;
    uint32_t num = stats.total_pages;
    if (num == 0) {
        num_str[pos++] = '0';
    } else {
        char temp[32];
        int temp_pos = 0;
        while (num > 0) {
            temp[temp_pos++] = '0' + (num % 10);
            num /= 10;
        }
        for (int j = temp_pos - 1; j >= 0; j--) {
            num_str[pos++] = temp[j];
        }
    }
    num_str[pos] = '\0';
    write_string(num_str);
    write_string("\n");
    
    // Display used pages
    write_string("  Used pages: ");
    pos = 0;
    num = stats.used_pages;
    if (num == 0) {
        num_str[pos++] = '0';
    } else {
        char temp[32];
        int temp_pos = 0;
        while (num > 0) {
            temp[temp_pos++] = '0' + (num % 10);
            num /= 10;
        }
        for (int j = temp_pos - 1; j >= 0; j--) {
            num_str[pos++] = temp[j];
        }
    }
    num_str[pos] = '\0';
    write_string(num_str);
    write_string("\n");
    
    // Display free pages
    write_string("  Free pages: ");
    pos = 0;
    num = stats.free_pages;
    if (num == 0) {
        num_str[pos++] = '0';
    } else {
        char temp[32];
        int temp_pos = 0;
        while (num > 0) {
            temp[temp_pos++] = '0' + (num % 10);
            num /= 10;
        }
        for (int j = temp_pos - 1; j >= 0; j--) {
            num_str[pos++] = temp[j];
        }
    }
    num_str[pos] = '\0';
    write_string(num_str);
    write_string("\n");
    
    // Display heap usage
    write_string("  Heap used: ");
    pos = 0;
    num = stats.heap_used;
    if (num == 0) {
        num_str[pos++] = '0';
    } else {
        char temp[32];
        int temp_pos = 0;
        while (num > 0) {
            temp[temp_pos++] = '0' + (num % 10);
            num /= 10;
        }
        for (int j = temp_pos - 1; j >= 0; j--) {
            num_str[pos++] = temp[j];
        }
    }
    num_str[pos] = '\0';
    write_string(num_str);
    write_string(" bytes\n");
}