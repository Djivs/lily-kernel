#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "VGAPrinter.hpp"
 
extern "C" void kernel_main(void) 
{
	VGAPrinter printer;

	printer.terminal_writestring("Welcome to the Lily's Kernel.\n");
}