void *memchr(const void *str, int c, unsigned int n) {
	unsigned int i;
	char *s = (char*)str;

	for (i = 0; i < n; ++i) {
		if (s[i] == c)
			return &s[i];
	}
	return 0;
}
