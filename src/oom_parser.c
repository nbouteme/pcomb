#include "parser.h"

void *mrealloc(void *b, int os, int ns)
{
	void *ret;

	ret = malloc(ns);
	memcpy(ret, b, os);
	free(b);
	return (ret);
}

void delete_oom(t_parser *base)
{
	t_oom_parser *self;

	self = (void*)base;
	delete_parser(self->repeat);
	self->repeat = 0;
}

int match_oom(t_parser *base, t_input *i, void **out)
{
	t_oom_parser *self;
	int m;
	void **buf;
	void *tmp;

	self = (void*)base;
	buf = 0;
	tmp = 0;
	m = 0;
	while (self->repeat->match_fun(self->repeat, i, &tmp))
	{
		buf = mrealloc(buf, m ? m * 8 : 0, (m + 1) * 8);
		buf[m++] = tmp;
	}
	if (m > 0)
	{
		free(*out);
		*out = self->folder(m, buf);
		free(buf);
		return 1;
	}
	else
		return 0;
}

t_parser *oom_parser(t_folder f, t_parser *to_repeat)
{
	t_oom_parser *ret = init_parser(malloc(sizeof(*ret)), match_oom, delete_oom);

	ret->repeat = to_repeat;
	ret->folder = f;
	ret->base.size = sizeof(*ret);
	return &ret->base;
}
