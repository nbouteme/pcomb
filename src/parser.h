#ifndef PARSER_H
# define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libft/std.h>

typedef struct s_parser t_parser;
typedef struct s_input t_input;

typedef void *t_err;

typedef int(*t_match_f)(t_parser *, t_input *, void**);
typedef void (*t_pdtor)(t_parser *);

typedef void *(*t_folder)(int, void **);
typedef void (*t_dtor)(void *);

typedef struct s_fpos
{
	int line;
	int col;
	int pos;
} t_fpos;

typedef struct s_ast
{
	char *tag;
	char *value;
	t_fpos loc;
	int n_children;
	struct s_ast **children;
} t_ast;

typedef struct s_input
{
	char *buffer;
	char *cursor;
	char *saved;
} t_input;

typedef struct s_parser
{
	char *name;
	t_match_f match_fun;
	t_pdtor dtor;
	int size;
	unsigned freeing;
} t_parser;

typedef struct s_string_parser
{
	t_parser base;
	char *to_parse;
} t_string_parser;

typedef struct s_and_parser
{
	t_parser base;
	t_folder folder;
	int n_and;
	t_dtor dtor;
	t_parser **to_and;
} t_and_parser;

typedef struct s_oom_parser
{
	t_parser base;
	t_parser *repeat;
	t_folder folder;
} t_oom_parser;

typedef struct s_char_parser
{
	t_parser base;
	char c;
} t_char_parser;

typedef struct s_or_parser
{
	t_parser base;
	int n_or;
	t_parser **to_or;
} t_or_parser;

typedef struct s_maybe_parser
{
	t_parser base;
	t_parser *p;
} t_maybe_parser;


typedef struct s_error
{
	t_fpos loc;
	char expected[];
} t_error;

typedef struct s_exp_p
{
	t_parser base;
	t_parser *p;
	char *e;
} t_exp_p;

typedef struct s_noneof_parser
{
	t_parser base;
	char *ign;
} t_noneof_parser;

typedef void *(*t_apply_fun)(void*);
typedef void *(*t_apply_fun_up)(void*, void*);

typedef struct s_apply_parser
{
	t_parser base;
	t_parser *p;
	t_apply_fun f;
} t_apply_parser;

typedef struct s_apply_up_parser
{
	t_parser base;
	t_parser *p;
	t_apply_fun_up f;
	void *up;
} t_apply_up_parser;

typedef struct s_statement
{
	char *name;
	t_parser *gr;
} t_statement;

typedef struct s_symtable
{
	t_dlisthead *syms;
} t_symtable;

typedef t_symtable t_langparser;

void do_nothing(t_parser *base);
void delete_parser(void *base);
void *init_parser(t_parser *p, t_match_f m, t_pdtor d);
void assign_parser(t_parser *a, t_parser *b);

t_parser *and_parser(t_dtor d, t_folder f, int n, t_parser *parser[]);
t_parser *string_parser(const char *str);
t_parser *oom_parser(t_folder f, t_parser *to_repeat);
t_parser *char_parser(char c);
t_parser *eof_parser();
t_parser *or_parser(int n, t_parser *parser[]);
t_parser *maybe_parser(t_parser *p);
t_parser *zom_parser(t_folder f, t_parser *p);
t_parser *nothing_parser();
t_parser *anychar_parser();
t_parser *noneof_parser(const char *ign);
t_parser *apply_parser(t_parser *i, t_apply_fun f);
t_parser *eof_parser();
t_parser *bof_parser();
t_parser *regex_parser(char *r);
t_parser *paren_parser(t_parser *p);
t_parser *apply_up_parser(t_parser *i, t_apply_fun_up f, void *up);
t_parser *cident_parser();
t_parser *tok_parser(t_parser *i);
t_parser *between_parser(t_parser *p, const char *l, const char *r);
t_parser *tok_between_parser(t_parser *p, const char *l, const char *r);
t_parser *tokbra_parser(t_parser *i);
t_parser *strlit_parser();
t_parser *charlit_parser();
t_parser *reglit_parser();
t_parser *oneof_parser();
t_parser *tag_p(t_parser *p, char *n);
t_parser *alnum_parser();
t_parser *alpha_parser();
t_parser *blank_parser();
t_parser *exp_p(t_parser *p, const char *e);
t_parser *escape_parser();
t_parser *loc_state(void);
t_parser *loc_ast_state(t_parser *p);
t_parser *root_p(t_parser *p);
t_parser *atag_p(t_parser *p, char *s);

t_langparser *lang_parser(char *lang);

void *ast_set_tag(void *a, void *v);
void *gen_char_parser(void *i);
void *ast_genstring(void *str);
void *ast_genchar(void *str);
void *ast_genident(void *str);
void *ast_genregex(void *str);
void *ast_genpref(void *rn, void *tgp);
void *save_rule_to(void *rn, void *tgp);

t_fpos *get_istate(t_input *i);
t_error *new_error(t_input *i, const char *e);

void *strfold(int n, void **b);
void *fold_or(int n, void **o);
void *fold_and(int n, void **o);
void *fold_rep(int n, void **o);
void *fold_anda(int n, void **o);
void *fold_repa(int n, void **o);
void *sm_fold(int n, void **o);
void *eval_first(int n, void **b);
void *ast_putstate(int n, void **b);
void *eval_second(int n, void **b);
void *astfold(int n, void **o);

t_ast *ast_add_child(t_ast *r, t_ast *c);
void *ast_add_root(void *o);
t_ast *ast_add_root_tag(t_ast *a, const char *t);
t_ast *ast_new(const char *key, const char *val);
void ast_print(t_ast *a);
void *ast_putstate(int n, void **b);
void *ast_putstr(void *s);
void *ast_set_tag(void *s, void *t);
void *ast_add_tag(void *s, void *t);
void delete_ast(t_ast *a);

void ast_delete_no_children(t_ast *a);

void delete_langparser(t_langparser *s);
void delete_ast2(void *a);
void do_nothing(t_parser *base);

char *file2buf(const char *file);
t_parser *get_parser(t_symtable *sy, char *sn);
t_dlisthead **get_plist();
int find_ptr(t_dlist *a, void *b);
char *str_escape(char *s, const char *in[], const char *out[]);
void delete_langparser(t_langparser *s);
void delete_parsers();

int run_parser(t_parser *p, char *s, void **res);

#endif
