#include "parser.h"

int match_string(t_parser *base, t_input *i, void **out)
{
	char *to_cmp;
	t_string_parser *self;

	self = (void*)base;
	to_cmp = self->to_parse;
	if (strncmp(to_cmp, i->cursor, strlen(to_cmp)))
		return (0);
	i->cursor += strlen(to_cmp);
	*out = strdup(to_cmp);
	return (1);
}

void delete_string(t_parser *base)
{
	t_string_parser *self;

	self = (void*)base;
	free(self->to_parse);
}

t_parser *string_parser(const char *str)
{
	t_string_parser *ret;

	ret = init_parser(malloc(sizeof(*ret)), match_string, delete_string);
	ret->to_parse = strdup(str);
	return &ret->base;
}
