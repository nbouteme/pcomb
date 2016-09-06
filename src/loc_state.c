#include "parser.h"

t_fpos *get_istate(t_input *i)
{
	int j;
	t_fpos *ret;

	ret = calloc(1, sizeof(*ret));
	ret->pos = i->cursor - i->buffer;
	j = 0;
	while (j < ret->pos)
	{
		if (i->buffer[j] == '\n')
		{
			ret->col = 0;
			++ret->line;
		}
		else
			ret->col++;
		++j;
	}
	return ret;
}

int state_match(t_parser *base, t_input *i, void **out)
{
	(void)base;
	*out = get_istate(i);
	return 1;
}

t_parser *loc_state()
{
	return init_parser(malloc(sizeof(t_parser)), state_match, do_nothing);
}

t_parser *loc_ast_state(t_parser *p)
{
	return and_parser(free, ast_putstate, 2,
					(t_parser*[]){loc_state(), p});
}
