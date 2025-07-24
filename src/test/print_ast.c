#include "ast/ast.h"
#include "expansion/expander.h" 
#include "error/error.h"
#include <stdio.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN	"\033[1;36m"

static const char *redir_str(t_redir_type type)
{
	if (type == RD_IN) return "<";
	if (type == RD_OUT) return ">";
	if (type == RD_APPEND) return ">>";
	if (type == RD_HEREDOC) return "<<";
	return "?";
}

static void	print_prefix(const char *prefix, int is_last)
{
	printf("%s", prefix);
	printf("%s── ", is_last ? "└" : "├");
}

void	short_print_tokens(t_token *tokens);

typedef int16_t (*t_expand_cb)(t_sh_ctx *ctx, t_ast *node, t_token **errtok);

static void	print_ast_generic(t_ast *node, const char *prefix, int is_last, t_sh_ctx *ctx, t_expand_cb expand_cb)
{
	char new_prefix[256];

	if (!node)
	{
		print_prefix(prefix, is_last);
		printf("(null)\n");
		return;
	}

	if (expand_cb)
	{
		t_token	*errtok = NULL;
		bool	exp;

		exp = true;
		if (node->type == ND_CMD)
			exp = node->u_data.cmd.is_expanded;
		else if (node->type == ND_REDIR)
			exp = node->u_data.rd.is_expanded;
		if (!exp && expand_cb(ctx, node, &errtok))
			err_print(PARSE_OK, errtok, false);
	}

	print_prefix(prefix, is_last);
	snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "│   ");

	if (node->type == ND_CMD)
	{
		printf(YELLOW "CMD:" RESET);
		if (node->u_data.cmd.is_expanded)
		{
			printf("[");
			for (size_t i = 0; node->u_data.cmd.args[i].expanded != NULL; i++)
				printf(i == 0 ? "%s" : ", (%s)", node->u_data.cmd.args[i].expanded);
			printf("]");
		}
		else
			short_print_tokens(node->u_data.cmd.args->tk);
		printf("\n");
	}
	else if (node->type == ND_PIPE)
	{
		printf(BLUE "PIPE" RESET "\n");
		print_ast_generic(node->u_data.op.left, new_prefix, 0, ctx, expand_cb);
		print_ast_generic(node->u_data.op.right, new_prefix, 1, ctx, expand_cb);
	}
	else if (node->type == ND_AND)
	{
		printf(GREEN "AND" RESET "\n");
		print_ast_generic(node->u_data.op.left, new_prefix, 0, ctx, expand_cb);
		print_ast_generic(node->u_data.op.right, new_prefix, 1, ctx, expand_cb);
	}
	else if (node->type == ND_OR)
	{
		printf(MAGENTA "OR" RESET "\n");
		print_ast_generic(node->u_data.op.left, new_prefix, 0, ctx, expand_cb);
		print_ast_generic(node->u_data.op.right, new_prefix, 1, ctx, expand_cb);
	}
	else if (node->type == ND_REDIR)
	{
		const char *expand_hd = "";

		if (node->u_data.rd.redir_type == RD_HEREDOC)
			expand_hd = node->u_data.rd.hd_expand ? "[expand]" : "[no-expand]";

		printf(RED "REDIR (%s)%s ->", redir_str(node->u_data.rd.redir_type), expand_hd);
		if (node->u_data.rd.is_expanded)
			printf(" %s", node->u_data.rd.filename.expanded);
		else
			short_print_tokens(node->u_data.rd.filename.tk);
		printf(RESET "\n");

		print_ast_generic(node->u_data.rd.child, new_prefix, 1, ctx, expand_cb);
	}
	else if (node->type == ND_SUBSHELL)
	{
		printf(CYAN "SUBSHELL" RESET "\n");
		print_ast_generic(node->u_data.subsh.child, new_prefix, 1, ctx, expand_cb);
	}
}

void	print_ast(t_ast *node)
{
	print_ast_generic(node, "", \
		node->type == ND_AND || node->type == ND_OR || node->type == ND_PIPE, NULL, NULL);
}

void	print_expanded_ast(t_ast *node, t_sh_ctx *ctx)
{
	print_ast_generic(node, "", \
		node->type == ND_AND || node->type == ND_OR || node->type == ND_PIPE, ctx, expand_node);
}

