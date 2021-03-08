#include "stdio.h"

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
	std::print("Hello, kernel World!\n");
}
