#include "terminal.h"

#if defined(__linux__)
#error "Use your cross-compiler, not host gcc"
#endif
#if !defined(__i386__)
#error "This needs to be built with an i386-elf compiler"
#endif


void kernel_main(void) {
	terminal_initialize();
	terminal_writestring("Hello from kernel\ntest");
}
