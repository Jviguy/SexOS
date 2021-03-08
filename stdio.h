#include "kernel.h"

namespace std {
	void print(const char* str) {
		terminal_writestring(str);
	}
}
