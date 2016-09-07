#include "../parser.h"

void *astfold(int n, void **o)
{
	int i, j;
	t_ast** as;
	t_ast *r;

	as = (t_ast**)o;
	if (n <= 1)
		return n ? o[0] : 0;
	if (n == 2 && (!o[0] || !o[1]))
		return o[0] ? o[0] : o[1];
	r = ast_new(">", "");
	i = -1;
	while (++i < n)
		if (as[i])
		{
			if (as[i]->n_children <= 1)
				ast_add_child(r, as[i]->n_children ?
				ast_add_root_tag(as[i]->children[0], as[i]->tag) : as[i]);
			else if ((j = -1))
				while (++j < as[i]->n_children)
					ast_add_child(r, as[i]->children[j]);
			if (as[i]->n_children)
				ast_delete_no_children(as[i]);
		}
	r->loc = r->n_children ? r->children[0]->loc : r->loc;
	return r;
}
