#include "stdio.h"
#include "string.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
	/* Initialize terminal interface */
	terminal_initialize();

	/* Since there is no support for newlines in terminal_putchar
         * yet, '\n' will produce some VGA specific character instead.
         * his is normal.
         */
	char msg[100];
	std::strcpy(msg, const_cast<char*>("Hello, kernel World!\n"));
	std::strcat(msg,  const_cast<char*>(std::AnsiColors().RED));
	std::print(msg);
}
