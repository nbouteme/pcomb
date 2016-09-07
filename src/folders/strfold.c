#include "../parser.h"

/*
** Concats a string arrays. Frees each reference in the array.
** @n: number of elements in the array
** @b: string array
 */
void *strfold(int n, void **b)
{
	char *ret;
	int len;
	int i;

	i = 0;
	len = 0;
	while (i < n)
	{
		len += strlen(b[i] ? b[i] : "");
		++i;
	}
	ret = malloc(len + 1);
	*ret = 0;
	i = 0;
	while (i < n)
	{
		strcat(ret, b[i] ? b[i] : "");
		free(b[i++]);
	}
	return ret;
}
