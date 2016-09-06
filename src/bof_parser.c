#include "parser.h"

int match_bof(t_parser *base, t_input *i, void **out)
{
	(void)base;
	(void)out;
	return (i->cursor == i->buffer);
}

t_parser *bof_parser()
{
	t_parser *ret;

	ret = malloc(sizeof(*ret));
	ret->match_fun = match_bof;
	ret->dtor = do_nothing;
	ret->size = sizeof(*ret);
	return ret;
}
