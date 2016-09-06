#include "parser.h"

t_parser *alnum_parser()
{
	return oneof_parser("0123456789abcdefghijklmnopqrstuvwxyz"
						"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

t_parser *alpha_parser()
{
	return oneof_parser("abcdefghijklmnopqrstuvwxyz"
						"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}
