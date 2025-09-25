#include "yield.h"
#include "../process.h"

// Function to execute yield command from shell
void execute_yield_command(void (*write_string)(const char*)) {
    write_string("Yielding CPU...\n");
    process_yield();
    write_string("Back in shell after yield.\n");
}