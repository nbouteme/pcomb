#include "parser.h"

void delete_ast(t_ast *a);

void delete_ast2(void *a)
{
	delete_ast(a);
}

void *strfold(int n, void **b)
{
	char *ret;
	int len;
	int i;

	i = 0;
	len = 0;
	while (i < n)
	{
		len += strlen(b[i] ? b[i] : "");
		++i;
	}
	ret = malloc(len + 1);
	*ret = 0;
	i = 0;
	while (i < n)
	{
		strcat(ret, b[i] ? b[i] : "");
		free(b[i++]);
	}
	return ret;
}

void *eval_nth_f(int x, int n, void **b)
{
	void *ret;

	ret = x <= n ? b[x] : 0;
	int i = 0;
	while (i < n)
		if(i != x)
			free(b[i++]);
		else
			++i;
	return ret;
}

void *eval_second(int n, void **b)
{
	return eval_nth_f(1, n, b);
}

void *eval_first(int n, void **b)
{
	return eval_nth_f(0, n, b);
}

int parse_input(t_input *i, t_parser *p, void **res)
{
	return p->match_fun(p, i, res);
}

void free_ast(t_ast *ast)
{
	int i;

	free(ast->tag);
	free(ast->value);
	i = 0;
	while (i < ast->n_children)
		free_ast(ast->children[i++]);
	free(ast);
}

void do_nothing(t_parser *base)
{
	(void)base;
}

void *fold_or(int n, void **o)
{
	(void) n;
	void *ret;

	if (!o[1])
		ret = o[0];
	else
		ret = or_parser(2, (t_parser*[]){o[0], o[1]});
	return ret;
}

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

void ast_delete_no_children(t_ast *a)
{
	free(a->children);
	free(a->tag);
	free(a->value);
	free(a);
}

t_ast *ast_new(const char *key, const char *val)
{
	t_ast *ret;

	ret = calloc(1, sizeof(*ret));
	ret->tag = strdup(key);
	ret->value = strdup(val);
	return ret;
}

t_ast *ast_add_child(t_ast *r, t_ast *c)
{
	void *tmp;

	tmp = r->children;
	r->children = malloc(sizeof(void*) * (r->n_children + 1));
	memcpy(r->children, tmp, sizeof(void*) * r->n_children);
	r->children[r->n_children++] = c;
	free(tmp);
	return r;
}

t_ast *ast_add_root_tag(t_ast *a, const char *t)
{
	if (a)
	{
		a->tag = realloc(a->tag, strlen(t) + strlen(a->tag) + 1);
		memmove(a->tag + strlen(t), a->tag, strlen(a->tag) + 1);
		memmove(a->tag, t, strlen(t));
	}
	return a;
}

void *astfold(int n, void **o)
{
	int i, j;
	t_ast** as;
	t_ast *r;

	as = (t_ast**)o;
	if (n <= 1)
		return n ? o[0] : 0;
	if (n == 2 && (!o[0] || !o[1]))
		return o[0] ? o[0] : o[1];
	r = ast_new(">", "");
	i = -1;
	while (++i < n)
		if (as[i])
		{
			if (as[i]->n_children <= 1)
				ast_add_child(r, as[i]->n_children ?
				ast_add_root_tag(as[i]->children[0], as[i]->tag) : as[i]);
			else if ((j = -1))
				while (++j < as[i]->n_children)
					ast_add_child(r, as[i]->children[j]);
			if (as[i]->n_children)
				ast_delete_no_children(as[i]);
		}
	r->loc = r->n_children ? r->children[0]->loc : r->loc;
	return r;
}

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

void nop(void*a){(void)a;}

void delete_ast(t_ast *a);

void *fold_anda(int n, void **o)
{
	return and_parser(delete_ast2, astfold, n, (t_parser**)o);
}

void *fold_and(int n, void **o)
{
	return and_parser(free, strfold, n, (t_parser**)o);
}

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

void *sm_fold(int n, void **b)
{
	void **ret;

	if (!b)
		return 0;
	ret = memcpy(malloc((n + 1) * sizeof(void*)), b, n * sizeof(void*));
	ret[n] = 0;
	return ret;
}

typedef struct s_statement
{
	char *name;
	t_parser *gr;
} t_statement;

int stmt_name_f(t_dlist *a, void *n)
{
	t_statement *d;

	d = a->content;
	return (strcmp(d->name, n) == 0);
}

typedef struct s_symtable
{
	t_dlisthead *syms;
} t_symtable;

t_parser *get_parser(t_symtable *sy, char *sn)
{
	t_parser *p;
	t_dlist *e;

	e = ftext_lstfind(sy->syms, stmt_name_f, sn);
	if (e)
		return ((t_statement*)e->content)->gr;
	p = init_parser(malloc(sizeof(*p) + 0x30), 0, do_nothing);
	p->size += 0x30;
	p->name = strdup(sn);
	e = ftext_lstnewelem(&(t_statement){strdup(sn), p}, sizeof(t_statement));
	ftext_lstpush_back(sy->syms, e);
	return p;
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
void *gen_statement(int n, void **b)
{
	t_statement *stmt;

	if (n != 4)
		return 0;
	stmt = calloc(1, sizeof(*stmt));
	stmt->name = strdup(b[0]);
	free(b[0]);
	stmt->gr = b[2];
	free(b[1]);
	free(b[3]);
	//free(b);
	return stmt;
}

void *ast_putstr(void *s)
{
	t_ast *ret;

	ret = ast_new("", s);
	free(s);
	return ret;
}

void *ast_putstate(int n, void **b)
{
	t_fpos *p;
	t_ast *a;

	(void)n;
	p = b[0];
	a = b[1];
	a->loc = *p;
	free(p);
	return a;
}

void *ast_set_tag(void *s, void *t)
{
	t_ast *ret;

	ret = s;
	free(ret->tag);
	ret->tag = strdup(t);
	return ret;
}

int find_in(const char *n, const char **h)
{
	int i;

	i = 0;
	while (h[i])
	{
		if (strncmp(n, h[i], strlen(h[i])) == 0)
			return i;
		++i;
	}
	return -1;
}

char *str_escape(char *s, const char *in[], const char *out[])
{
	char *outp;
	char *f;
	char *tmp;
	int ol;
	int i;

	outp = calloc(1, strlen(s));
	ol = 0;
	f = outp;
	while (*s)
	{
		if ((i = find_in(s, in)) != -1)
		{
			tmp = outp;
			outp = calloc(1, strlen(tmp) + strlen(out[i]) + 1);
			strcat(outp, tmp);
			strcat(outp, out[i]);
			ol += strlen(out[i]);
			s += strlen(in[i]);
			f = outp;
		}
		else
			f[ol++] = *s++;
	}
	return outp;
}

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

void *ast_add_root(void *o)
{
	t_ast *a;
	t_ast *r;

	a = o;
	if (!a)
		return 0;
	if (a->n_children <= 1)
		return a;
	r = ast_new(">", "");
	ast_add_child(r, a);
	return r;
}

t_parser *root_p(t_parser *p)
{
	return apply_parser(p, ast_add_root);
}

void *ast_genpref(void *rn, void *tgp)
{
	t_parser *p;

	p = get_parser(tgp, rn);
	free(rn);
	return loc_ast_state(root_p(tag_p(p, p->name)));
}

typedef t_symtable t_langparser;

void delete_parsers();

t_langparser *lang_parser(char *il)
{
	t_parser *lg, *sm, *gr, *te, *fa, *ba;
	t_symtable *tg;
	void *ret;

	tg = malloc(sizeof(*tg));
	tg->syms = ftext_lstnew();
	t_parser *p;
	p = tok_parser(cident_parser());
	p = loc_ast_state(tag_p(apply_parser(p, ast_putstr), "identb"));
	assign_parser(get_parser(tg, "name"), p);
	p = tok_parser(cident_parser());
	p = loc_ast_state(tag_p(apply_parser(p, ast_putstr), "identc"));
	assign_parser(get_parser(tg, "string"), p);
	lg = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	sm = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	gr = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	te = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	fa = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	ba = init_parser(malloc(sizeof(t_parser) + 0x30), 0, do_nothing);
	assign_parser(lg, apply_up_parser(zom_parser(sm_fold, sm), save_rule_to, tg));
	assign_parser(sm, and_parser(delete_parser, gen_statement, 4, (t_parser*[]){
	tok_parser(cident_parser()), string_parser(":="), gr, char_parser(';')}));
	assign_parser(gr, and_parser(delete_parser, fold_or, 2, (t_parser*[]){
		te, maybe_parser(and_parser(delete_parser, eval_second, 2, (t_parser*[])
		{char_parser('|'), gr}))}));
	assign_parser(te, oom_parser(fold_anda, fa));
	assign_parser(fa, and_parser(delete_parser, fold_repa, 2, (t_parser*[]){ba,
		or_parser(2, (t_parser*[]){oneof_parser("*+?!"), nothing_parser()})}));
	assign_parser(ba, or_parser(5, (t_parser*[]){
		apply_parser(tok_parser(strlit_parser()), ast_genstring),
		apply_parser(tok_parser(charlit_parser()), ast_genchar),
		apply_parser(tok_parser(reglit_parser()), ast_genregex),
		apply_up_parser(tokbra_parser(cident_parser()), ast_genpref, tg),
		paren_parser(gr)}));
	run_parser(lg, il, &ret);
	delete_parser(lg);
	delete_parsers();
	return tg;
}

void delete_parser_node(void *n, unsigned long ign)
{
	t_statement *s;
	(void)ign;
	(void)n;
	s = n;
	free(s->name);
	free(s);
}

void markfordelete(t_dlist *l)
{
	t_statement *s;

	s = l->content;
	delete_parser(s->gr);
}

void delete_langparser(t_langparser *s)
{
	ftext_lstiter(s->syms, markfordelete);
	ftext_lstdel(&s->syms, delete_parser_node);
	delete_parsers();
	free(s);
}

void delete_ast(t_ast *a)
{
	int i;

	if (!a)
		return ;
	i = 0;
	while (i < a->n_children)
		delete_ast(a->children[i++]);
	free(a->children);
	free(a->tag);
	free(a->value);
	free(a);
}

void ast_printr(t_ast *a, int d)
{
	int i;

	i = 0;
	if (!a)
		return ;
	while (i++ < d)
		printf("   ");
	i = 0;
	if (strlen(a->value))
		printf("%s -> %d:%d %s\n", a->tag, a->loc.line, a->loc.col, a->value);
	else
		puts(a->tag);
	while(i < a->n_children)
		ast_printr(a->children[i++], d + 1);
}

void ast_print(t_ast *a)
{
	ast_printr(a, 0);
}

#include <unistd.h>
#include <sys/fcntl.h>

char *file2buf(const char *file)
{
	char *ret;
	char *f;
	int r;

	ret = calloc(1, 512);
	f = ret;
	int fd = open(file, O_RDONLY);
	while((r = read(fd, f, 512)) > 0)
		f += r;
	close(fd);
	return ret;
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
		puts("An error occured");
		printf("%p\n", out);
	}
	else
		ast_print(out);
	free(desc);
	free(input);
	delete_ast(out);
	delete_langparser(and);
    return 0;
}
