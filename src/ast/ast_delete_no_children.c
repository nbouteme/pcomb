#include "../parser.h"

void ast_delete_no_children(t_ast *a)
{
	free(a->children);
	free(a->tag);
	free(a->value);
	free(a);
}
