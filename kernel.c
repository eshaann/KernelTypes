#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "Use your cross-compiler, not host gcc"
#endif
#if !defined(__i386__)
#error "This needs to be built with an i386-elf compiler"
#endif


void handleNewLine(void);
static void updateCursorPosition(size_t row, size_t col);

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) VGA_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_putchar(char c) {
	const size_t index = terminal_row * VGA_WIDTH + terminal_column;
	switch(c){
		case '\n':
			handleNewLine();
			break;
		default:
			terminal_buffer[index] = vga_entry(c, terminal_color);
			if (++terminal_column == VGA_WIDTH) {
				//terminal_column = 0;
			}	
			break;
	}

	updateCursorPosition(terminal_row, terminal_column);
}

void terminal_writestring(const char* str) {
	for (size_t i = 0; str[i] != '\0'; i++) {
		terminal_putchar(str[i]);
	}
}

void kernel_main(void) {
	terminal_initialize();
	terminal_writestring("Hello from kernel\ntest");
}

void handleNewLine(void){
	terminal_column = 0;
	if (++terminal_row == VGA_HEIGHT) {
		terminal_column = 0;
		for(int y = 1; y < VGA_HEIGHT; y++){
			for(int x = 0; x < VGA_WIDTH; x++){
				terminal_buffer[(y-1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
			}
		}
		for(int x = 0; x < VGA_WIDTH; x++){
			terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
		}
		terminal_row = VGA_HEIGHT - 1;
	}
}

static inline void outb(uint16_t port, uint8_t val){
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void updateCursorPosition(size_t row, size_t col){
	uint16_t position = row * VGA_WIDTH + col;

	//low byte of cursor pos
	outb(0x3D4, 0x0F);
	//modify 
	outb(0x3D5, (uint8_t)(position & 0xFF));

	//high byte of pos
	outb(0x3D4, 0x0E);
	//modify
	outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));

}