#include "timer.h"

static volatile uint32_t timer_ticks = 0;

void timer_initialize() {
    timer_ticks = 0;
}

uint32_t timer_get_ticks() {
    // Simple implementation using RDTSC (lower 32 bits)
    uint32_t low;
    asm volatile ("rdtsc" : "=a" (low) : : "edx");
    return low;
}