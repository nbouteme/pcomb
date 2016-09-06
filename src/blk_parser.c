#include "parser.h"

t_parser *wht_parser()
{
	return oneof_parser(" \n\t\r\v\f");	
}

t_parser *whts_parser()
{
	return zom_parser(strfold, wht_parser());	
}

t_parser *blank_parser()
{
	return whts_parser();
}
