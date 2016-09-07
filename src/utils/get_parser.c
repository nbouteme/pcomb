#include "../parser.h"

static int stmt_name_f(t_dlist *a, void *n)
{
	t_statement *d;

	d = a->content;
	return (strcmp(d->name, n) == 0);
}

t_parser *get_parser(t_symtable *sy, char *sn)
{
	t_parser *p;
	t_dlist *e;

	e = ftext_lstfind(sy->syms, stmt_name_f, sn);
	if (e)
		return ((t_statement*)e->content)->gr;
	p = init_parser(malloc(sizeof(*p) + 0x30), 0, do_nothing);
	p->size += 0x30;
	p->name = strdup(sn);
	e = ftext_lstnewelem(&(t_statement){strdup(sn), p}, sizeof(t_statement));
	ftext_lstpush_back(sy->syms, e);
	return p;
}
