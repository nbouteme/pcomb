#include "parser.h"

int run_parser(t_parser *p, char *s, void **res)
{
	int ret;

	t_input i;
	*res = 0;
	i.buffer = s;
	i.cursor = s;
	ret = p->match_fun(p, &i, res);
	return ret;
}

void *gen_char_parser(void *i)
{
	char s;

	s = *(char*)i;
	free(i);
	return char_parser(s);
}

void *save_rule_to(void *res, void *up)
{
	t_statement **sts;
	t_statement *st;
	t_symtable *sy;
	t_parser *p;

	sts = res;
	sy = up;
	while (*sts)
	{
		st = *sts;
		p = get_parser(sy, st->name);
		assign_parser(p, st->gr);
		free(st->name);
		free(st);
		++sts;
	}
	free(res);
	return 0;
}

#include <stdio.h>

void print_error(t_error *e, const char *i)
{
	t_fpos s = {0, 0, 0};

	s.line = 1;
	while (s.line != e->loc.line)
		if (*i++ == '\n')
			++s.line;
	while (*i && *i != '\n')
	{
		putchar((*i == '\t' ? ' ' : *i));
		++i;
	}
	putchar('\n');
	while (s.col++ != e->loc.col)
		putchar(' ');
	putchar('^');
	putchar('\n');
}

int main(int ac, char **av)
{
	if (ac != 4)
		return 2;
	char *input, *desc;
	desc = file2buf(av[1]);
	input = file2buf(av[3]);
	t_langparser *and = lang_parser(desc);
	t_parser *p = get_parser(and, av[2]);
	void *out = 0;
	if (!run_parser(p, input, &out))
	{
		t_error *kek = out;
		printf("%s", kek->expected);
		print_error(out, input);
		free(kek);
	}
	else
	{
		ast_print(out);
		delete_ast(out);
	}
	delete_langparser(and);
	free(desc);
	free(input);
    return 0;
}
