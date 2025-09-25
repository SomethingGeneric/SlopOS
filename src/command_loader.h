#ifndef COMMAND_LOADER_H
#define COMMAND_LOADER_H

#include "types.h"
#include "filesystem.h"

// Command process entry points
extern "C" void version_process_main();
extern "C" void hello_process_main();
extern "C" void help_process_main();
extern "C" void ps_process_main();
extern "C" void uptime_process_main();
extern "C" void memory_process_main();
extern "C" void yield_process_main();

// Command loader functions
void command_loader_init();
uint32_t command_spawn_process(const char* command_name);
bool command_is_available(const char* command_name);

#endif