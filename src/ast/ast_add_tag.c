#include "../parser.h"

void *ast_add_tag(void *s, void *t)
{
	t_ast *ret;
	char *str;

	ret = s;
	str = calloc(1, strlen(ret->tag) + 1 + strlen(t) + 1);
	strcat(str, ret->tag);
	strcat(str, "|");
	strcat(str, t);
	free(ret->tag);
	ret->tag = str;
	return ret;
}
