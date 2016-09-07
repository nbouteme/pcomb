#include "../parser.h"

/*
** Returns a parser that accepts only input for which the n
** parsers in the o arrays sequentially accept the given input
** Expect the parsers to all return an AST fragment.
 */
void *fold_anda(int n, void **o)
{
	return and_parser(delete_ast2, astfold, n, (t_parser**)o);
}

/*
** Same as fold_anda except for the following:
** Expect the parsers to all return a string
*/
void *fold_and(int n, void **o)
{
	return and_parser(free, strfold, n, (t_parser**)o);
}
