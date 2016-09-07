#include "../parser.h"

t_dlisthead **get_plist()
{
	static t_dlisthead *p = 0;
	return &p;
}

int find_ptr(t_dlist *a, void *b)
{
	return a->content == b;
}
