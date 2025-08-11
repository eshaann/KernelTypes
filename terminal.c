#include "terminal.h"
#include "vga.h"
#include "io.h"

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

void updateCursorPosition(size_t row, size_t col){
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