#include "ast/ast.h"
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

static void	print_ast_rec(t_ast *node, const char *prefix, int is_last)
{
	char new_prefix[256];

	if (!node)
	{
		print_prefix(prefix, is_last);
		printf("(null)\n");
		return;
	}
	
	print_prefix(prefix, is_last);
	snprintf(new_prefix, 256, "%s%s", prefix, is_last ? "    " : "│   ");

	if (node->type == ND_CMD)
	{
		printf(YELLOW "CMD:" RESET);
		for (int i = 0; node->u_data.s_cmd.argv && node->u_data.s_cmd.argv[i]; i++)
			printf(" '%s'", node->u_data.s_cmd.argv[i]);
		printf("\n");
	}
	else if (node->type == ND_PIPE)
	{
		printf(BLUE "PIPE" RESET "\n");
		print_ast_rec(node->u_data.op.left, new_prefix, 0);
		print_ast_rec(node->u_data.op.right, new_prefix, 1);
	}
	else if (node->type == ND_AND)
	{
		printf(GREEN "AND" RESET "\n");
		print_ast_rec(node->u_data.op.left, new_prefix, 0);
		print_ast_rec(node->u_data.op.right, new_prefix, 1);
	}
	else if (node->type == ND_OR)
	{
		printf(MAGENTA "OR" RESET "\n");
		print_ast_rec(node->u_data.op.left, new_prefix, 0);
		print_ast_rec(node->u_data.op.right, new_prefix, 1);
	}
	else if (node->type == ND_REDIR)
	{
		const char	*expand_hd;

		expand_hd = "";
		if (node->u_data.rd.redir_type == RD_HEREDOC)
			expand_hd = node->u_data.rd.hd_expand ? "[expand]" : "[no-expand]";
		printf(RED "REDIR (%s)%s -> '%s'" RESET "\n",
			redir_str(node->u_data.rd.redir_type),
			expand_hd,
			node->u_data.rd.filename);
		print_ast_rec(node->u_data.rd.child, new_prefix, 1);
	}
	else if (node->type == ND_SUBSHELL)
	{
		printf(CYAN "SUBSHELL" RESET "\n");
		print_ast_rec(node->u_data.s_subsh.child, new_prefix, 1);
	}
}

void	print_ast_ascii(t_ast *node)
{
	print_ast_rec(node, "", \
		node->type == ND_AND || node->type == ND_OR || node->type == ND_PIPE);
}
