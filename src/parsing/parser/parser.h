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

# define HANDLED_CHAR	"()<>|&'\""

//? Forward declaration
typedef struct s_ast		t_ast;
typedef enum e_node_type	t_node_type;

// Parser

t_ast	*logical_parser(t_token **cur, t_token **errtok);
t_ast	*redir_parser(t_token **cur, t_token **errtok);
t_ast	*primary_parser(t_token **cur, t_token **errtok);

// Argv helpers
char	**join_argv(char **og, char **new);
char	**collect_argv(t_token **cur, t_token **errtok);

#endif
