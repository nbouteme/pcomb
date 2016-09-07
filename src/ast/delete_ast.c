#include "../parser.h"

void delete_ast(t_ast *a)
{
	int i;

	if (!a)
		return ;
	i = 0;
	while (i < a->n_children)
		delete_ast(a->children[i++]);
	free(a->children);
	free(a->tag);
	free(a->value);
	free(a);
}
