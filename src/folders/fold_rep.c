#include "../parser.h"

/*
** Returns a repetition combination of the first parser in the array
** depending on the symbol represented by the symbol of the second element in
** the array.
** Expects the array elements to point to parsers.
** Expects the parser to return a string
** Expects the second element to be a string
** The string argument is free'd.
*/

void *fold_rep(int n, void **o)
{
	void *ret;

	(void) n;
	ret = 0;
	if (!o[1])
		ret = o[0];
	else if (strcmp(o[1], "*") == 0)
		ret = zom_parser(strfold, o[0]);
	else if (strcmp(o[1], "+") == 0)
		ret = oom_parser(strfold, o[0]);
	else if (strcmp(o[1], "?") == 0)
		ret = maybe_parser(o[0]);
	free(o[1]);
	return ret;
}

/*
** Same as fold_rep except for the following modifications.
** Expects the parser to return an AST fragment
*/

void *fold_repa(int n, void **o)
{
	void *ret;

	(void) n;
	ret = 0;
	if (!o[1])
		ret = o[0];
	else if (strcmp(o[1], "*") == 0)
		ret = zom_parser(astfold, o[0]);
	else if (strcmp(o[1], "+") == 0)
		ret = oom_parser(astfold, o[0]);
	else if (strcmp(o[1], "?") == 0)
		ret = maybe_parser(o[0]);
	free(o[1]);
	return ret;
}
