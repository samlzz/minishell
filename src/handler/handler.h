/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:15:57 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/31 09:35:42 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLER_H
# define HANDLER_H

# include <stdint.h>

extern volatile int32_t	g_sig;

typedef enum e_sig_handle
{
	SIGH_MAIN,
	SIGH_RESTORE,
	SIGH_HD,
	SIGH_RUNNING_CH,
	SIGH_RUNNING_BI,
}	t_sig_handle;

void	sig_init(t_sig_handle action);

#endif
