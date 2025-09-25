#include "../syscall.h"
#include "../terminal.h"
#include "../process.h"
#include "../string.h"

// Process entry point for ps command
extern "C" void ps_process_main() {
    terminal_writestring("Process list:\n");
    
    // Get real process information
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_t* proc = process_get_by_index(i);
        if (proc) {
            terminal_writestring("  PID ");
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
            
            terminal_writestring(pid_str);
            terminal_writestring(": ");
            terminal_writestring(proc->name);
            
            // Show state
            switch (proc->state) {
                case PROCESS_RUNNING:
                    terminal_writestring(" (running)\n");
                    break;
                case PROCESS_READY:
                    terminal_writestring(" (ready)\n");
                    break;
                case PROCESS_BLOCKED:
                    terminal_writestring(" (blocked)\n");
                    break;
                case PROCESS_TERMINATED:
                    terminal_writestring(" (terminated)\n");
                    break;
            }
        }
    }
    
    // Exit this process
    sys_exit(0);
}