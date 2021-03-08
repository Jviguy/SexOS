#include "stdio.h"
#include "string.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
	/* Initialize terminal interface */
	terminal_initialize();

	char msg[100];
	std::strcpy(msg, const_cast<char*>(std::AnsiColors().RED));
	std::strcat(msg, const_cast<char*>("Hello, kernel World!"));
	std::strcat(msg, const_cast<char*>(std::AnsiColors().RED));
	std::print(msg);
}
