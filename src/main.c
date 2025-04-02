#include <stdio.h>
#include <string.h>
#include "ast.h"

static inline const char	*token_type_str(t_tk_type type)
{
	if (type == TK_WORD) return "WORD";
	if (type == TK_PIPE) return "PIPE";
	if (type == TK_AND) return "AND";
	if (type == TK_OR) return "OR";
	if (type == TK_REDIR_IN) return "REDIR_IN";
	if (type == TK_REDIR_OUT) return "REDIR_OUT";
	if (type == TK_REDIR_APPEND) return "REDIR_APPEND";
	if (type == TK_HEREDOC) return "HEREDOC";
	if (type == TK_EOF) return "EOF";
	return "UNKNOWN";
}

static inline const char	*quote_type_str(t_quote_type type)
{
	if (type == QUOTE_DOUBLE) return "DOUBLE QUOTE";
	if (type == QUOTE_SINGLE) return "SINGLE QUOTE";
	if (type == QUOTE_NONE) return "NONE QUOTE";
	return "nAn";
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TK_WORD)
			printf("Token: %s %-18s | Value: \"%s\"\n",
				token_type_str(tokens->type),
				quote_type_str(tokens->quote),
				tokens->value);
		else
			printf("Token: %-23s | Value: \"%s\"\n",
				token_type_str(tokens->type),
				tokens->value);
		tokens = tokens->next;
	}
}

void	print_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == ND_CMD)
	{
		printf("[CMD] ");
		for (int i = 0; node->u_data.s_cmd.argv && node->u_data.s_cmd.argv[i]; i++)
			printf("'%s' ", node->u_data.s_cmd.argv[i]);
		printf("\n");
	}
	else if (node->type == ND_REDIR)
	{
		printf("[REDIR] ");
		if (node->u_data.s_redir.redir_type == RD_IN) printf("< ");
		else if (node->u_data.s_redir.redir_type == RD_OUT) printf("> ");
		else if (node->u_data.s_redir.redir_type == RD_APPEND) printf(">> ");
		else if (node->u_data.s_redir.redir_type == RD_HEREDOC) printf("<< ");
		printf("'%s'\n", node->u_data.s_redir.filename);
		print_ast(node->u_data.s_redir.child);
	}
	else if (node->type == ND_PIPE)
	{
		printf("[PIPE]\n");
		print_ast(node->u_data.s_binop.left);
		print_ast(node->u_data.s_binop.right);
	}
	else if (node->type == ND_AND)
	{
		printf("[AND]\n");
		print_ast(node->u_data.s_binop.left);
		print_ast(node->u_data.s_binop.right);
	}
	else if (node->type == ND_OR)
	{
		printf("[OR]\n");
		print_ast(node->u_data.s_binop.left);
		print_ast(node->u_data.s_binop.right);
	}
}


int	main(void)
{
	char	input[1024];
	t_ast	*ast;
	int16_t	err;

	while (1)
	{
		printf("miniparse> ");
		if (!fgets(input, sizeof(input), stdin))
			break;

		// Remove trailing newline
		input[strcspn(input, "\n")] = '\0';

		err = new_ast((const char *)input, &ast);
		if (!ast)
		{
			fprintf(stderr, "Erreur : échec de la création de l'ast. %d\n", err);
			continue;
		}
		print_ast(ast);
		ast_free(ast);
	}
	printf("Bye!\n");
	return (0);
}

