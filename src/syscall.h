#ifndef SYSCALL_H
#define SYSCALL_H

#include "types.h"

// System call numbers
#define SYS_EXIT        1
#define SYS_WRITE       2
#define SYS_READ        3
#define SYS_YIELD       4

// System call interface
void syscall_init();
int syscall_handler(uint32_t call_num, uint32_t arg1, uint32_t arg2, uint32_t arg3);

// System call implementations
int sys_exit(int status);
int sys_write(const char* buffer, size_t count);
int sys_read(char* buffer, size_t count);
int sys_yield();

#endif