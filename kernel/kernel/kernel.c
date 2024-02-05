#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	printf("Welcome to the Lily's kernel.\n");
	printf("/txt_col_5The bridge to Ether.\n");
}
