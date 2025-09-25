#ifndef UPTIME_H
#define UPTIME_H

// Function to execute uptime command from shell
void execute_uptime_command(void (*write_string)(const char*));

#endif