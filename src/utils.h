/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:12:41 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/27 10:02:53 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdbool.h>
# include <stdint.h>

//? Forward declaration
typedef enum e_tk_type		t_tk_type;
typedef enum e_quote_type	t_quote_type;
typedef enum e_redir_type	t_redir_type;

typedef struct s_argword	t_argword;
typedef struct s_dynint		t_dynint;

char	*ft_getinput(char const *prompt, bool is_main);

// string
bool	ft_isspace(int c);
char	*ft_str3join(const char *s1, const char *s2, const char *s3);
bool	ft_strjreplace(char **target, const char *suff);

#endif
