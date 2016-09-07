#include "../parser.h"

static int find_in(const char *n, const char **h)
{
	int i;

	i = 0;
	while (h[i])
	{
		if (strncmp(n, h[i], strlen(h[i])) == 0)
			return i;
		++i;
	}
	return -1;
}

char *str_escape(char *s, const char *in[], const char *out[])
{
	char *outp;
	char *f;
	char *tmp;
	int ol;
	int i;

	outp = calloc(1, strlen(s));
	ol = 0;
	f = outp;
	while (*s)
	{
		if ((i = find_in(s, in)) != -1)
		{
			tmp = outp;
			outp = calloc(1, strlen(tmp) + strlen(out[i]) + 1);
			strcat(outp, tmp);
			strcat(outp, out[i]);
			ol += strlen(out[i]);
			s += strlen(in[i]);
			f = outp;
		}
		else
			f[ol++] = *s++;
	}
	return outp;
}
