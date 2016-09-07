#include "../parser.h"

static void *mrealloc(void *b, int os, int ns)
{
	void *ret;

	ret = malloc(ns);
	memcpy(ret, b, os);
	free(b);
	return (ret);
}

int match_zom(t_parser *base, t_input *i, void **out)
{
	t_oom_parser *self;
	int m;
	void **buf;
	void *tmp;

	self = (void*)base;
	buf = 0;
	tmp = 0;
	m = 0;
	*out = 0;
	while (self->repeat->match_fun(self->repeat, i, &tmp))
	{
		buf = mrealloc(buf, m * 8, (m + 1) * 8);
		buf[m++] = tmp;
	}
	free(tmp);
	*out = 0;
	*out = self->folder(m, buf);
	free(buf);
	return 1;
}


void delete_oom(t_parser *base);

t_parser *zom_parser(t_folder f, t_parser *p)
{
	t_oom_parser *ret = init_parser(malloc(sizeof(*ret)), match_zom, delete_oom);

	ret->folder = f;
	ret->repeat = p;
	ret->base.size = sizeof(*ret);
	return &ret->base;
}
