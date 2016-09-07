#include "../parser.h"

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
	return stmt;
}

t_langparser *lang_parser(char *il)
{
	t_parser *lg, *sm, *gr, *te, *fa, *ba;
	t_symtable *tg;
	void *ret;

	tg = malloc(sizeof(*tg));
	tg->syms = ftext_lstnew();
	assign_parser(get_parser(tg, "name"), loc_ast_state(tag_p(apply_parser(tok_parser(cident_parser()), ast_putstr), "identifier")));
	assign_parser(get_parser(tg, "string"), loc_ast_state(tag_p(apply_parser(tok_parser(cident_parser()), ast_putstr), "identifier")));
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
		apply_up_parser(tok_parser(tokbra_parser(cident_parser())), ast_genpref, tg),
		paren_parser(gr)}));
	run_parser(lg, il, &ret);
	delete_parser(lg);
	delete_parsers();
	return tg;
}
