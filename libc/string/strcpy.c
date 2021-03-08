#include <string.h>

char* strcpy(char* to, char* from) {
	char *save = to;
	for (; (*to = *from); ++from, ++to);
	return(save);
}
