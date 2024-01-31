#include "VGAPrinter.hpp"

#include "string.hpp"

VGAPrinter::VGAPrinter() {
    terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void VGAPrinter::terminal_writestring(const char* data) 
{
	terminal_write(data, string::strlen(data));
}

void VGAPrinter::terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void VGAPrinter::terminal_putchar(char c) 
{
    if (c == '\n') {
        terminal_column = VGA_WIDTH;
    } else {
	    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        ++terminal_column;
    }

	if (terminal_column == VGA_WIDTH) {
		terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            --terminal_row;
            move_terminal_rows_up();
            // terminal_row = 0;
        } 
	}

   
}

void VGAPrinter::terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void VGAPrinter::terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void VGAPrinter::move_terminal_rows_up() {
    for (size_t row = 1; row < VGA_HEIGHT; ++row) {
        for (size_t col = 0; col < VGA_WIDTH; ++col) {
            const size_t upper_index = (row - 1) *VGA_WIDTH + col;
            const size_t lower_index = row * VGA_WIDTH + col;

            terminal_buffer[upper_index] = terminal_buffer[lower_index];
        }
    }

    for (size_t col = 0; col < VGA_WIDTH; ++col) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + col;

        terminal_buffer[index] = ' '; 
    }
}

inline uint8_t VGAPrinter::vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

inline uint16_t VGAPrinter::vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}