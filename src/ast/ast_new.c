#include "../parser.h"

t_ast *ast_new(const char *key, const char *val)
{
	t_ast *ret;

	ret = calloc(1, sizeof(*ret));
	ret->tag = strdup(key);
	ret->value = strdup(val);
	return ret;
}
