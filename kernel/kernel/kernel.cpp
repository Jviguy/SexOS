#include <stdio.h>
 
#include <kernel/tty.h>

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
