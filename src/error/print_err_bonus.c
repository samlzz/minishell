/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:47:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 10:46:36 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

#ifdef MINISHELL_BONUS

const char	*token_type_str(t_token *errtok)
{
	const char	*stringify[TK_EOF + 1];

	if (!errtok \
		|| errtok->type < TK_WORD \
		|| errtok->type > TK_EOF)
		return ("unknow");
	stringify[TK_WORD] = "WORD";
	stringify[TK_PIPE] = "|";
	stringify[TK_AND] = "&&";
	stringify[TK_OR] = "||";
	stringify[TK_LPAREN] = "(";
	stringify[TK_RPAREN] = ")";
	stringify[TK_REDIR_IN] = "<";
	stringify[TK_REDIR_OUT] = ">";
	stringify[TK_REDIR_APPEND] = ">>";
	stringify[TK_HEREDOC] = "<<";
	stringify[TK_EOF] = "newline";
	return (stringify[errtok->type]);
}

#endif
