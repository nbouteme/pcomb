#include "../parser.h"

void *ast_add_root(void *o)
{
	t_ast *a;
	t_ast *r;

	a = o;
	if (!a)
		return 0;
	if (a->n_children <= 1)
		return a;
	r = ast_new(">", "");
	ast_add_child(r, a);
	return r;
}
