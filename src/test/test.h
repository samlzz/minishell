/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:29:22 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/27 16:46:03 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include "parser/parser.h"
# include "env/env.h"


# define PRINT_TOKENS			1

# define PRINT_AST				1
// ? if want to adjust set to 0 ^
# define PRINT_AST_NO_EXPAND	1
# define PRINT_AST_EXPAND		0

// * Functions

void	print_tokens(t_token *tokens);
void	short_print_tokens(t_token *tokens);

void	print_ast(t_ast *node);
void	print_expanded_ast(t_ast *node, t_sh_ctx *ctx);

#endif
