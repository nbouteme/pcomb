#include "../parser.h"

int match_maybe(t_parser *base, t_input *i, void **out)
{
	char *curr;
	t_maybe_parser *self;

	self = (void*)base;
	curr = i->cursor;
	if (!self->p->match_fun(self->p, i, out))
	{
		i->cursor = curr;
		free(*out);
		*out = 0;
	}
	return (1);
}

void delete_maybe(t_parser *base)
{
	t_maybe_parser *self;

	self = (void*)base;
	delete_parser(self->p);
	self->p = 0;
}

t_parser *maybe_parser(t_parser *p)
{
	t_maybe_parser *ret = init_parser(malloc(sizeof(*ret)), match_maybe, delete_maybe);

	ret->base.size = sizeof(*ret);
	ret->p = p;
	return &ret->base;
}
