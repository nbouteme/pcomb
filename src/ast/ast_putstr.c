#include "../parser.h"

void *ast_putstr(void *s)
{
	t_ast *ret;

	ret = ast_new("", s);
	free(s);
	return ret;
}
