#pragma once

#include <stdint.h>
#include <stddef.h>

namespace string {
    extern "C" size_t strlen(const char* str)  {
    	size_t len = 0;
    	while (str[len])
    		len++;
    	return len;
    }
}