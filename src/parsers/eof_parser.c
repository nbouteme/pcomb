#include "../parser.h"

int match_eof(t_parser *base, t_input *i, void **out)
{
	(void)base;
	(void)out;
	return (i->cursor >= (i->buffer + strlen(i->buffer)));
}

t_parser *eof_parser()
{
	t_parser *ret;

	ret = malloc(sizeof(*ret));
	ret->match_fun = match_eof;
	ret->dtor = do_nothing;
	ret->size = sizeof(*ret);
	return exp_p(ret, "<EOF>");
}
