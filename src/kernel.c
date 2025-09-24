// SlopOS Kernel - Simple C implementation
// Echo kernel that repeats user input

// Basic types
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long size_t;

// VGA text mode constants
#define VGA_BUFFER ((volatile uint16_t*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR_WHITE_ON_BLACK 0x0F

// Global state
static size_t cursor_x = 0;
static size_t cursor_y = 0;

// Basic I/O functions
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            cursor_y = 0; // Simple wrap around
        }
    } else if (c == '\b' && cursor_x > 0) {
        cursor_x--;
        size_t pos = cursor_y * VGA_WIDTH + cursor_x;
        VGA_BUFFER[pos] = (VGA_COLOR_WHITE_ON_BLACK << 8) | ' ';
    } else if (c >= 32) {
        size_t pos = cursor_y * VGA_WIDTH + cursor_x;
        VGA_BUFFER[pos] = (VGA_COLOR_WHITE_ON_BLACK << 8) | c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
            if (cursor_y >= VGA_HEIGHT) {
                cursor_y = 0;
            }
        }
    }
}

void print(const char* str) {
    while (*str) {
        putchar(*str);
        str++;
    }
}

char getchar(void) {
    // Simple scancode to ASCII map
    static char scancode_to_ascii[58] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0, ' '
    };
    
    while (1) {
        if (inb(0x64) & 0x01) {
            uint8_t scancode = inb(0x60);
            if (!(scancode & 0x80) && scancode < 58 && scancode_to_ascii[scancode]) {
                return scancode_to_ascii[scancode];
            }
        }
    }
}

void kernel_main(void) {
    char buffer[256];
    int pos = 0;
    char c;
    
    // Clear screen
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_BUFFER[i] = (VGA_COLOR_WHITE_ON_BLACK << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
    
    // Welcome message
    print("SlopOS Simple C Kernel\n");
    print("Type text and press Enter - it will be echoed back!\n\n");
    
    // Main echo loop
    while (1) {
        print("> ");
        pos = 0;
        
        // Read input line
        while (1) {
            c = getchar();
            
            if (c == '\n') {
                buffer[pos] = '\0';
                putchar('\n');
                break;
            } else if (c == '\b' && pos > 0) {
                pos--;
                putchar('\b');
            } else if (c >= 32 && c <= 126 && pos < 255) {
                buffer[pos++] = c;
                putchar(c);
            }
        }
        
        // Echo the input
        print("Echo: ");
        print(buffer);
        print("\n");
    }
}