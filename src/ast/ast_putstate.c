#include "../parser.h"

void *ast_putstate(int n, void **b)
{
	t_fpos *p;
	t_ast *a;

	(void)n;
	p = b[0];
	a = b[1];
	a->loc = *p;
	free(p);
	return a;
}
