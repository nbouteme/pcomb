#include "../parser.h"

int match_apply_up(t_parser *base, t_input *i, void **out)
{
	t_apply_up_parser *self;
	void *tmp;
	int ret;

	self = (void*)base;
	if ((ret = self->p->match_fun(self->p, i, out)))
	{
		tmp = *out;
		*out = self->f(tmp, self->up);
	}
	return ret;
}

void delete_apply(t_parser *base);

t_parser *apply_up_parser(t_parser *i, t_apply_fun_up f, void *up)
{
	t_apply_up_parser *ret = init_parser(malloc(sizeof(*ret)), match_apply_up, delete_apply);

	ret->p = i;
	ret->f = f;
	ret->up = up;
	ret->base.size = sizeof(*ret);
	return &ret->base;
}
