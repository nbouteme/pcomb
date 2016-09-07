#include "../parser.h"

int match_noneof(t_parser *base, t_input *i, void **out)
{
	t_noneof_parser *self;

	self = (void*)base;
	if(strchr(self->ign, i->cursor[0]))
		return 0;
	if (*i->cursor)
	{
		*out = strdup(" ");
		*(char*)*out = *i->cursor++;;
	}
	else
		*out = strdup("<EOF>");
	return 1;
}

void delete_noneof(t_parser *base)
{
	t_noneof_parser *self;

	self = (void*)base;
	free(self->ign);
}

t_parser *noneof_parser(const char *ign)
{
	t_noneof_parser *ret = init_parser(malloc(sizeof(*ret)), match_noneof, delete_noneof);
	char *e;

	ret->ign = strdup(ign);
	ret->base.size = sizeof(*ret);
	e = malloc(strlen("Expected nothing in `") + strlen(ign) + 2);
	strcpy(e, "Expected nothing in `");
	strcat(e, ign);
	strcat(e, "`");
	ret = (void*)exp_p(&ret->base, e);
	free(e);
	return &ret->base;
}
