#ifndef TYPES_H
#define TYPES_H

// Basic type definitions for freestanding environment
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned long size_t;

// Boolean type
#ifndef __cplusplus
typedef enum { false = 0, true = 1 } bool;
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

#endif