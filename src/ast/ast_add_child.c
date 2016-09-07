#include "../parser.h"

t_ast *ast_add_child(t_ast *r, t_ast *c)
{
	void *tmp;

	tmp = r->children;
	r->children = malloc(sizeof(void*) * (r->n_children + 1));
	memcpy(r->children, tmp, sizeof(void*) * r->n_children);
	r->children[r->n_children++] = c;
	free(tmp);
	return r;
}
