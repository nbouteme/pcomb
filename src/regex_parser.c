#include "parser.h"

t_parser *regex_engine()
{
	t_parser *regex;
	t_parser *term;
	t_parser *factor;
	t_parser *base;

	regex  = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	term  = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	factor  = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	base  = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	assign_parser(regex, and_parser(delete_parser, fold_or, 2,
	(t_parser*[]){term, maybe_parser(and_parser(delete_parser, eval_second, 2,
		(t_parser*[]){char_parser('|'), regex}))}));
	assign_parser(term, zom_parser(fold_and, factor));
	assign_parser(factor, and_parser(delete_parser, fold_rep, 2, (t_parser*[]){base,
		or_parser(4, (t_parser*[]){	char_parser('*'),
									char_parser('+'),
									char_parser('?'),
									nothing_parser() })}));
	assign_parser(base, or_parser(2, (t_parser*[]){paren_parser(regex),
					apply_parser(noneof_parser(")|"), gen_char_parser)}));
	return regex;
}

t_parser *regex_parser(char *r)
{
	void *out;
	t_parser *re;

	re = regex_engine();
	if (!run_parser(re, r, &out))
		exit(43);
	delete_parser(re);
	return out;
}
