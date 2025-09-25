#include "../terminal.h"
#include "../process.h"
#include "../timer.h"

// Uptime process - runs as separate process
extern "C" void uptime_process() {
    // For now, run once and terminate
    process_t* current = process_get_current();
    if (current) {
        char msg[] = "UPTIME: System uptime: ";
        terminal_writestring(msg);
        
        uint32_t ticks = timer_get_ticks();
        char uptime_str[32];
        int pos = 0;
        if (ticks == 0) {
            uptime_str[pos++] = '0';
        } else {
            char temp[32];
            int temp_pos = 0;
            while (ticks > 0) {
                temp[temp_pos++] = '0' + (ticks % 10);
                ticks /= 10;
            }
            // Reverse
            for (int j = temp_pos - 1; j >= 0; j--) {
                uptime_str[pos++] = temp[j];
            }
        }
        uptime_str[pos] = '\0';
        terminal_writestring(uptime_str);
        terminal_writestring(" ticks (separate process)\n");
        
        process_terminate(current->pid);
    }
}

// Function to execute uptime command from shell
void execute_uptime_command(void (*write_string)(const char*)) {
    write_string("Creating uptime process...\n");
    uint32_t uptime_pid = process_create("uptime", uptime_process, 1);
    if (uptime_pid == 0) {
        write_string("Failed to create uptime process!\n");
    } else {
        write_string("Uptime process created successfully.\n");
        // Yield to allow the uptime process to run
        process_yield();
    }
}