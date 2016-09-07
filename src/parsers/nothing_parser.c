#include "../parser.h"

int match_nothing(t_parser *base, t_input *i, void **out)
{
	(void)base;
	(void)i;
	*out = 0;
	return (1);
}

t_parser *nothing_parser()
{
	t_parser *ret = init_parser(malloc(sizeof(*ret)), match_nothing, do_nothing);

	ret->size = sizeof(*ret);
	return ret;
}
