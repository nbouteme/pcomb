#include "../parser.h"

static void ast_printr(t_ast *a, int d)
{
	int i;

	i = 0;
	if (!a)
		return ;
	while (i++ < d)
		printf("   ");
	i = 0;
	if (strlen(a->value))
		printf("%s -> %d:%d %s\n", a->tag, a->loc.line, a->loc.col, a->value);
	else
		puts(a->tag);
	while(i < a->n_children)
		ast_printr(a->children[i++], d + 1);
}

void ast_print(t_ast *a)
{
	ast_printr(a, 0);
}
