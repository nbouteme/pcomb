#define _GNU_SOURCE
#include "../parser.h"

t_error *new_error(t_input *i, const char *e)
{
	char *str;
	t_fpos *pos = get_istate(i);
	t_error *ret;

	asprintf(&str, "Expected `%s` at %d:%d\n", e, pos->line, pos->col);
	ret = malloc(sizeof(*ret) + strlen(str) + 1);
	ret->loc = *pos;
	free(pos);
	strcpy(ret->expected, str);
	free(str);
	return ret;
}

int exp_match(t_parser *base, t_input *i, void **out)
{
	t_exp_p *self;
	void *tmp;

	self = (void*)base;
	tmp = i->cursor;
	*out = 0;
	if (self->p->match_fun(self->p, i, out))
		return (1);
	free(*out);
	*out = new_error(i, self->e);
	i->cursor = tmp;
	return (0);
}

void delete_exp(t_parser *p)
{
	t_exp_p *s;

	s = (void*)p;
	delete_parser(s->p);
	free(s->e);
}

t_parser *exp_p(t_parser *p, const char *e)
{
	t_exp_p *ret;

	ret = init_parser(malloc(sizeof(*ret)), exp_match, delete_exp);
	ret->base.size = sizeof(*ret);
	ret->e = strdup(e);
	ret->p = p;
	return &ret->base;
}
