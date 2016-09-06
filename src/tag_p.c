#include "parser.h"

t_parser *tag_p(t_parser *p, char *s)
{
	return apply_up_parser(p, ast_set_tag, s);
}
