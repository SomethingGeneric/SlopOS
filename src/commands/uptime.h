#ifndef UPTIME_H
#define UPTIME_H

// Uptime process entry point
extern "C" void uptime_process();

// Function to execute uptime command from shell
void execute_uptime_command(void (*write_string)(const char*));

#endif