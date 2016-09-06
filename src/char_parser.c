#include "parser.h"
#include <execinfo.h>

int match_char(t_parser *base, t_input *i, void **out)
{
	t_char_parser *self;

	self = (void*)base;
	if(i->cursor[0] != self->c)
		return 0;
	i->cursor++;
	if (self->c)
	{
		*out = strdup(" ");
		*(char*)*out = self->c;
	}
	else
		*out = strdup("<EOF>");
	return 1;
}

t_parser *char_parser(char c)
{
	t_char_parser *ret = init_parser(malloc(sizeof(*ret)), match_char, do_nothing);

	ret->base.dtor = do_nothing;
	ret->base.match_fun = match_char;
	ret->c = c;
	ret->base.size = sizeof(*ret);
	return &ret->base;
}
