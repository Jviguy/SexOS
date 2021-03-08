#include <kernel.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main() {
	/* Initialize terminal interface */
	terminal_initialize();

	terminal_writestring("Hellow world!");
}
