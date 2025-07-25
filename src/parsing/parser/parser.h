/* ************************************************************************** */
/*	                                                                        */
/*	                                                    :::      ::::::::   */
/*   parser.h	                                       :+:      :+:    :+:   */
/*	                                                +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>	      +#+  +:+       +#+        */
/*	                                            +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 19:56:14 by sliziard	      #+#    #+#             */
/*   Updated: 2025/03/31 19:56:15 by sliziard	     ###   ########.fr       */
/*	                                                                        */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer/token.h"
# include "utils/utils.h"
# include <stdio.h>

//? Forward declaration
typedef struct s_ast		t_ast;
typedef enum e_node_type	t_node_type;
typedef union u_words		t_words;

// Parser

t_ast	*logical_parser(t_token **cur, t_token **errtok);
t_ast	*redir_parser(t_token **cur, t_token **errtok);
t_ast	*primary_parser(t_token **cur, t_token **errtok);

// Helpers

t_words	*collect_args(t_token **cur, t_token **errtok);

void	*parse_err(char const *msg, t_ast *to_free);

#endif
