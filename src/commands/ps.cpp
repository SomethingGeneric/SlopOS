#include "../process.h"
#include "../string.h"

// Function to execute ps command from shell
void execute_ps_command(void (*write_string)(const char*)) {
    write_string("Process list:\n");
    // Get real process information
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_t* proc = process_get_by_index(i);
        if (proc) {
            write_string("  PID ");
            char pid_str[16];
            // Simple integer to string conversion
            int pid = proc->pid;
            int pos = 0;
            if (pid == 0) {
                pid_str[pos++] = '0';
            } else {
                char temp[16];
                int temp_pos = 0;
                while (pid > 0) {
                    temp[temp_pos++] = '0' + (pid % 10);
                    pid /= 10;
                }
                // Reverse
                for (int j = temp_pos - 1; j >= 0; j--) {
                    pid_str[pos++] = temp[j];
                }
            }
            pid_str[pos] = '\0';
            write_string(pid_str);
            write_string(": ");
            write_string(proc->name);
            
            // Show state
            switch (proc->state) {
                case PROCESS_RUNNING:
                    write_string(" (running)\n");
                    break;
                case PROCESS_READY:
                    write_string(" (ready)\n");
                    break;
                case PROCESS_BLOCKED:
                    write_string(" (blocked)\n");
                    break;
                case PROCESS_TERMINATED:
                    write_string(" (terminated)\n");
                    break;
            }
        }
    }
}