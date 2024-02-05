#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <kernel/tty.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* __restrict__ format, ...) {
	va_list parameters;
	va_start(parameters, format);

	static const char* text_color_change_cmd = "/txt_col_";
	static const char* back_color_change_cmd = "/back_col_";

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			if (!memcmp(format, text_color_change_cmd, strlen(text_color_change_cmd))) {
				const uint8_t cmd_len = strlen(text_color_change_cmd);
				const char color_num = format[cmd_len];
				terminal_set_text_color(color_num - '0');
				format += cmd_len + 1;
				amount -= (cmd_len + 1);
			}

			if (!memcmp(format, text_color_change_cmd, strlen(back_color_change_cmd))) {
				const uint8_t cmd_len = strlen(back_color_change_cmd);
				const char color_num = format[cmd_len];
				terminal_set_text_color(color_num - '0');
				format += cmd_len + 1;
				amount -= (cmd_len + 1);
			}
			
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			

			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
