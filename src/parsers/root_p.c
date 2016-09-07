#include "../parser.h"

/*
** Returns a parsers that sets the AST fragment returned by p to
** be the root of a tree.
** Expects p to return an AST fragment
*/

t_parser *root_p(t_parser *p)
{
	return apply_parser(p, ast_add_root);
}
