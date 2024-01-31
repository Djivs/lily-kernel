#pragma once

#include <stdint.h>
#include <stddef.h>



namespace time {
extern "C" {
    
    void sleep_ms(uint32_t milliseconds) {
        for (size_t i = 0; i < 1000 * 1000 * milliseconds; ++i) {
			asm volatile("nop");
		}    
    }
    
    void sleep_s(uint32_t seconds) {
        sleep_ms(1000 * seconds);
    }
}
}