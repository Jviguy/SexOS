#include <string.h>

char* strcat(char* s, char* append) {
	char *save = s;
	for (; *s; ++s);
	while ((*s++ = *append++));
	return save;
}
