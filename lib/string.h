namespace std {
	char *	strcpy(char* to, char* from) {
		char *save = to;
		for (; (*to = *from); ++from, ++to);
		return(save);
	}

	char* strcat(char* s, char* append) {
		char *save = s;
		for (; *s; ++s);
		while ((*s++ = *append++));
		return save;
	}

}
