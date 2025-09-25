#include "terminal.h"
#include "string.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    
    // Just clear first line instead of entire screen to minimize operations
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[x] = vga_entry(' ', terminal_color);
    }
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll() {
    // Move all lines up by one
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t src_index = (y + 1) * VGA_WIDTH + x;
            const size_t dst_index = y * VGA_WIDTH + x;
            terminal_buffer[dst_index] = terminal_buffer[src_index];
        }
    }
    
    // Clear the last line
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
    
    terminal_row = VGA_HEIGHT - 1;
    terminal_column = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
        }
    } else if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
            terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
        }
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                terminal_scroll();
            }
        }
    }
}

void terminal_writestring(const char* data) {
    for (size_t i = 0; i < strlen(data); i++) {
        terminal_putchar(data[i]);
    }
}

// Simple keyboard input using polling
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a" (val), "Nd" (port));
}

char terminal_getchar() {
    static const char scancode_to_ascii[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0, ' '
    };
    
    uint8_t scancode;
    uint8_t status;
    
    // Wait for keyboard data to be available
    // Check keyboard controller status register at port 0x64
    // Bit 0: Output buffer status (1 = data available from keyboard)
    // Bit 1: Input buffer status (1 = data has not been read yet)
    do {
        status = inb(0x64);
        // Wait until there's data available (bit 0 set) and input buffer is ready (bit 1 clear)
    } while (!(status & 0x01));
    
    // Read the scan code from the keyboard data port
    scancode = inb(0x60);
    
    // Only handle key press events (scan codes with bit 7 clear)
    if (scancode & 0x80) {
        // This is a key release event, ignore and try again
        return terminal_getchar();
    }
    
    // Convert scan code to ASCII if possible
    if (scancode < sizeof(scancode_to_ascii)) {
        char c = scancode_to_ascii[scancode];
        if (c != 0) {
            return c;
        }
    }
    
    // Unknown scan code or non-printable key, try again
    return terminal_getchar();
}

void terminal_getstring(char* buffer, size_t max_length) {
    size_t pos = 0;
    char c;
    
    while (pos < max_length - 1) {  // Leave space for null terminator
        c = terminal_getchar();
        
        if (c == '\n') {
            // Enter pressed - end input
            terminal_putchar('\n');
            break;
        } else if (c == '\b') {
            // Backspace pressed - remove character if possible
            if (pos > 0) {
                pos--;
                terminal_putchar('\b');
            }
        } else if (c >= 32 && c <= 126) {
            // Printable character - add to buffer and echo
            buffer[pos] = c;
            pos++;
            terminal_putchar(c);
        }
        // Ignore other characters (non-printable, special keys, etc.)
    }
    
    // Null terminate the string
    buffer[pos] = '\0';
}