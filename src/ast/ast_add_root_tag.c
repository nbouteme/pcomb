#include "../parser.h"

t_ast *ast_add_root_tag(t_ast *a, const char *t)
{
	if (a)
	{
		a->tag = realloc(a->tag, strlen(t) + strlen(a->tag) + 1);
		memmove(a->tag + strlen(t), a->tag, strlen(a->tag) + 1);
		memmove(a->tag, t, strlen(t));
	}
	return a;
}
