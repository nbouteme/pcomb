#include "../parser.h"

void delete_ast2(void *a)
{
	delete_ast(a);
}

void do_nothing(t_parser *base)
{
	(void)base;
}

static void delete_parser_node(void *n, unsigned long ign)
{
	t_statement *s;
	(void)ign;
	(void)n;
	s = n;
	free(s->name);
	free(s);
}

static void markfordelete(t_dlist *l)
{
	t_statement *s;

	s = l->content;
	delete_parser(s->gr);
}

void delete_langparser(t_langparser *s)
{
	ftext_lstiter(s->syms, markfordelete);
	ftext_lstdel(&s->syms, delete_parser_node);
	delete_parsers();
	free(s);
}
