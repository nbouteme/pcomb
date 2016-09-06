#include "parser.h"

t_parser *tokbra_parser(t_parser *p)
{
	return tok_between_parser(p, "<", ">");
}

t_parser *tok_parser(t_parser *p)
{
	return and_parser(free, eval_second, 3,
					  (t_parser*[]){blank_parser(), p, blank_parser()});
}
