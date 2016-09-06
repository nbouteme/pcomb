#include "parser.h"

int match_oneof(t_parser *base, t_input *i, void **out)
{
	t_noneof_parser *self;

	self = (void*)base;
	if(!strchr(self->ign, i->cursor[0]))
		return 0;
	if (!*i->cursor)
		return 0;
	*out = strdup(" ");
	*(char*)*out = *i->cursor++;;
	return 1;
}

void delete_noneof(t_parser *base);

t_parser *oneof_parser(const char *ign)
{
	t_noneof_parser *ret = init_parser(malloc(sizeof(*ret)), match_oneof, delete_noneof);

	ret->ign = strdup(ign);
	ret->base.size = sizeof(*ret);
	return &ret->base;
}
