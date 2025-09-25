#include "hello.h"

// Function to execute hello command from shell
void execute_hello_command(void (*write_string)(const char*)) {
    write_string("world\n");
}