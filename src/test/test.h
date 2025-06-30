/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:29:22 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/30 09:16:36 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include "parser/parser.h"
# include "env/env.h"
# include <stdio.h>


# define PRINT_ENV				0

# define PRINT_TOKENS			0

# define PRINT_AST				0
// ? if want to adjust set to 0 ^
# define PRINT_AST_NO_EXPAND	0
# define PRINT_AST_EXPAND		1

# define PRINT_HD_COUNT			1
// Exec
# define PRINT_EXIT_CODE		1

static inline void	print_entry(char *key, void *val)
{
	if (*key == '@')
		printf("(INTERNAL):");
	printf("[%s]: %s\n", key, (char *)val);
}

void	print_tokens(t_token *tokens);
void	short_print_tokens(t_token *tokens);

void	print_ast(t_ast *node);
void	print_expanded_ast(t_ast *node, t_sh_ctx *ctx);

#endif
