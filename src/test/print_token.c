#include <stdio.h>
#include "parser/parser.h"

static inline const char	*_token_type_str(t_tk_type type)
{
	if (type == TK_WORD) return "WORD";
	if (type == TK_ASSIGN) return "ASSIGN";
	if (type == TK_PIPE) return "PIPE";
#ifdef MINISHELL_BONUS
	if (type == TK_AND) return "AND";
	if (type == TK_OR) return "OR";
	if (type == TK_LPAREN) return "BEGIN PARENTHESIS";
	if (type == TK_RPAREN) return "END PARENTHESIS";
#endif
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
			printf("Token: %s %-18s | Joined: %s\n\tValue: \"%s\"\n",
				_token_type_str(tokens->type),
				quote_type_str(tokens->quote),
				tokens->glued ? "true" : "false",
				tokens->value
			);
		else
			printf("Token: %-23s | Value: \"%s\"\n",
				_token_type_str(tokens->type),
				tokens->value);
		tokens = tokens->next;
	}
}

void	short_print_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TK_WORD)
		{
			printf(" %s(\"%s\"", _token_type_str(tokens->type), tokens->value);
			if (tokens->quote != QUOTE_NONE)
				printf(" | %s", quote_type_str(tokens->quote));
			if (tokens->glued)
				printf(" | glued");
			printf(")");
		}
		else
		{
			printf(" %s(\"%s\")", _token_type_str(tokens->type), tokens->value);
		}
		tokens = tokens->next;
	}
}
