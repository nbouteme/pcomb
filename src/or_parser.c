#include "parser.h"

int match_or(t_parser *base, t_input *i, void **out)
{
	t_or_parser *self;
	int idx;

	self = (void*)base;
	idx = 0;
	while (idx < self->n_or)
		if (self->to_or[idx]->match_fun(self->to_or[idx], i, out))
			return (1);
		else
			++idx;
	return (0);
}

void delete_or(t_parser *p)
{
	t_or_parser *self = (void*)p;
	int i;

	i = 0;
	while (i < self->n_or)
	{
		delete_parser(self->to_or[i]);
		self->to_or[i] = 0;
		++i;
	}
	free(self->to_or);
}

t_parser *or_parser(int n, t_parser *parser[])
{
	t_or_parser *ret;

	ret = init_parser(malloc(sizeof(*ret)), match_or, delete_or);
	ret->to_or = memcpy(malloc(n * sizeof(void*)), parser, n * sizeof(void*));
	ret->n_or = n;
	ret->base.size = sizeof(*ret);
	return &ret->base;
}
