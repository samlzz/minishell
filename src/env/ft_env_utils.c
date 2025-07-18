/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:20:49 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/18 13:34:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <unistd.h>

int16_t	env_literal_set(t_env *env, const char *key, const char *val)
{
	char	*tmp;

	if (val)
		tmp = ft_str3join(key, "=", val);
	else
		tmp = ft_strdup(key);
	if (!tmp)
		return (1);
	return (env_set(env, tmp));
}
