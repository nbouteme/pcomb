#include "../parser.h"

void *ast_genstring(void *str)
{
	const char *in_charset2[] = {"\a", "\b", "\n", "\t", "\v", "\f", "\r", "\\", "'", "\"", 0};
	const char *out_charset2[] = {"\\a", "\\b", "\\n", "\\t", "\\v", "\\f", "\\r", "\\\\", "\\'", "\\\"", 0};
	t_parser *p;
	char *o;

	o = str_escape(str, out_charset2, in_charset2);
	p = tok_parser(string_parser(str));
	free(o);
	free(str);
	return loc_ast_state(tag_p(apply_parser(p, ast_putstr), "string"));
}

void *ast_genchar(void *str)
{
	const char *in_charset[] = {"\a", "\b", "\n", "\t", "\v", "\f", "\r", "\\", "'", "\"", 0};
	const char *out_charset[] = {"\\a", "\\b", "\\n", "\\t", "\\v", "\\f", "\\r", "\\\\", "\\'", "\\\"", 0};
	t_parser *p;
	char *o;

	o = str_escape(str, out_charset, in_charset);
	p = tok_parser(string_parser(str));
	free(o);
	free(str);
	return loc_ast_state(tag_p(apply_parser(p, ast_putstr), "char"));
}

void *ast_genident(void *str)
{
	t_parser *p;

	p = tok_parser(string_parser(str));
	free(str);
	return loc_ast_state(tag_p(apply_parser(p, ast_putstr), "identa"));
}

void *ast_genregex(void *str)
{
	const char *in_charset[] = {"/", 0};
	const char *out_charset[] = {"\\/", 0};
	t_parser *p;
	char *o;

	o = str_escape(str, out_charset, in_charset);
	p = tok_parser(regex_parser(str));
	free(o);
	free(str);
	return loc_ast_state(tag_p(apply_parser(p, ast_putstr), "regex"));
}

void *ast_genpref(void *rn, void *tgp)
{
	t_parser *p;

	p = get_parser(tgp, rn);
	free(rn);
	return loc_ast_state(root_p(atag_p(p, p->name)));
}
