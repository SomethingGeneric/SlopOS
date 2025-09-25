#include "version.h"

// Function to execute version command from shell
void execute_version_command(void (*write_string)(const char*)) {
    write_string("slopOS 2.0 - Now with process management!\n");
}