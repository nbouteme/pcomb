#include "../parser.h"

t_parser *cident_parser()
{
	t_parser *a;	
	t_parser *b;

	a = or_parser(2, (t_parser*[]){alpha_parser(), char_parser('_')});
	b = zom_parser(strfold, or_parser(2, (t_parser*[]){alnum_parser(), char_parser('_')}));
	b = and_parser(delete_parser, strfold, 2, (t_parser*[]){a, b});
	return exp_p(b, "C-like identifier");
}
