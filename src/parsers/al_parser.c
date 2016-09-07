#include "../parser.h"

static t_parser *_alnum_parser()
{
	return oneof_parser("0123456789abcdefghijklmnopqrstuvwxyz"
						"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

static t_parser *_alpha_parser()
{
	return oneof_parser("abcdefghijklmnopqrstuvwxyz"
						"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

t_parser *alpha_parser()
{
	return exp_p(_alpha_parser(), "alpha char");
}

t_parser *alnum_parser()
{
	return exp_p(_alnum_parser(), "alpha char");
}
