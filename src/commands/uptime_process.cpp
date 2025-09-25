#include "../syscall.h"
#include "../terminal.h"
#include "../timer.h"

// Process entry point for uptime command
extern "C" void uptime_process_main() {
    terminal_writestring("System uptime: ");
    
    // Get timer ticks (simplified)
    uint32_t ticks = timer_get_ticks();
    uint32_t seconds = ticks / 1000; // Assuming 1000 Hz timer
    
    // Simple integer to string conversion for seconds
    char uptime_str[16];
    int pos = 0;
    if (seconds == 0) {
        uptime_str[pos++] = '0';
    } else {
        char temp[16];
        int temp_pos = 0;
        while (seconds > 0) {
            temp[temp_pos++] = '0' + (seconds % 10);
            seconds /= 10;
        }
        // Reverse
        for (int j = temp_pos - 1; j >= 0; j--) {
            uptime_str[pos++] = temp[j];
        }
    }
    uptime_str[pos] = '\0';
    
    terminal_writestring(uptime_str);
    terminal_writestring(" seconds\n");
    
    // Exit this process
    sys_exit(0);
}