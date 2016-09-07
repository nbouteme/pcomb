#include "../parser.h"

t_parser *strlit_parser()
{
	t_parser *strchar = or_parser(2, (t_parser*[]){escape_parser(), noneof_parser("\"")});
	return between_parser(oom_parser(strfold, strchar), "\"", "\"");
}

t_parser *charlit_parser()
{
	return between_parser(or_parser(2, (t_parser*[]){escape_parser(),
									anychar_parser()}), "'", "'");
}

t_parser *reglit_parser()
{
	t_parser *regexchar = or_parser(2, (t_parser*[]){escape_parser(), noneof_parser("/")});
	return between_parser(oom_parser(strfold, regexchar), "/", "/");
}
