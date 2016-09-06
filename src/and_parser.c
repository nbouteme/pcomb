#include "parser.h"

int match_and(t_parser *base, t_input *i, void **out)
{
	t_and_parser *self;
	int idx;
	void **buf;

	self = (void*)base;
	buf = calloc(self->n_and, sizeof(void*));
	idx = 0;
	while (idx < self->n_and)
	{
		if (self->to_and[idx]->match_fun(self->to_and[idx], i, &buf[idx]))
			++idx;
		else
		{
			*out = buf[idx];
			while (--idx >= 0)
				self->dtor(buf[idx]);
			free(buf);
			return (0);
		}
	}
	*out = self->folder(self->n_and, buf);
	free(buf);	
	return (1);
}

void delete_and(t_parser *base)
{
	t_and_parser *self;
	int i;

	self = (void*)base;
	i = 0;
	while (i < self->n_and)
	{
		delete_parser(self->to_and[i]);
		self->to_and[i] = 0;
		++i;
	}
	free(self->to_and);
}

t_parser *and_parser(t_dtor d, t_folder f, int n, t_parser *parser[])
{
	t_and_parser *ret;

	ret = init_parser(malloc(sizeof(*ret)), match_and, delete_and);
	ret->folder = f;
	ret->dtor = d;
	ret->to_and = memcpy(malloc(n * sizeof(void*)), parser, n * sizeof(void*));
	ret->n_and = n;
	ret->base.size = sizeof(*ret);
	return &ret->base;
}
