#include "../parser.h"

void *ast_set_tag(void *s, void *t)
{
	t_ast *ret;

	ret = s;
	free(ret->tag);
	ret->tag = strdup(t);
	return ret;
}
