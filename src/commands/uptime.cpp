#include "../terminal.h"
#include "../process.h"
#include "../timer.h"

// Function to execute uptime command from shell (simplified - no separate process)
void execute_uptime_command(void (*write_string)(const char*)) {
    write_string("UPTIME: System uptime: ");
    
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
    write_string(uptime_str);
    write_string(" ticks\n");
}