#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;

void handleNewLine(void);
void updateCursorPosition(size_t row, size_t col);
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_writestring(const char* str);

#endif