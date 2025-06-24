/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:29:22 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/24 19:42:08 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include "parser/parser.h"
# include <stdio.h>


# define PRINT_ENV				1

# define PRINT_TOKENS			1

# define PRINT_AST				1 
// ? if want to adjust set to 0 ^
# define PRINT_AST_NO_EXPAND	0
# define PRINT_AST_EXPAND		0

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

void	print_ast_ascii(t_ast *node);

#endif
