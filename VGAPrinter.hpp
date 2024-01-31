#pragma once

#include <stdint.h>
#include <stddef.h>

/* Hardware text mode color constants. */
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

extern "C" {
/*
 * This is meant to be a singleton class, but at the moment I don't have
 * any memory allocation (malloc etc), so i will just leave this class as it is.
 * 
*/
class VGAPrinter {
    public:
        VGAPrinter();

        void set_text_color(vga_color color);
        void set_background_color(vga_color color);
        
        void terminal_writestring(const char* data);
        void terminal_write(const char* data, size_t size);
        void terminal_putchar(char c);
    private:
        void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

        void move_terminal_rows_up();

        static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
        static inline uint16_t vga_entry(unsigned char uc, uint8_t color);
    private:
        size_t terminal_row;
        size_t terminal_column;
        uint8_t terminal_color;
        uint16_t* terminal_buffer;

        vga_color text_color;
        vga_color background_color;

        static const size_t VGA_WIDTH = 80;
        static const size_t VGA_HEIGHT = 25;
};

}
