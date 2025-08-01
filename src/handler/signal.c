/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:15:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/31 09:36:04 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

#include "handler.h"

volatile int32_t	g_sig = 0;

void	sigint_handler(int32_t sig)
{
	g_sig = sig;
	if (isatty(STDIN_FILENO))
		write(2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	child_sig_handler(int32_t sig)
{
	g_sig = sig;
}

void	hd_quit_handler(int32_t sig)
{
	g_sig = sig;
	rl_done = 1;
}

void	sig_init(t_sig_handle action)
{
	void	(*h)(int32_t);

	if (action == SIGH_MAIN)
	{
		signal(SIGINT, &sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
	}
	else if (action == SIGH_RUNNING_CH)
	{
		signal(SIGINT, &child_sig_handler);
		signal(SIGQUIT, &child_sig_handler);
	}
	else if (action == SIGH_HD)
	{
		signal(SIGINT, &hd_quit_handler);
	}
	else if (action == SIGH_RESTORE || action == SIGH_RUNNING_BI)
	{
		h = (void *)((action == SIGH_RESTORE) * (uintptr_t)SIG_DFL
				+ (action == SIGH_RUNNING_BI) * (uintptr_t)SIG_IGN);
		signal(SIGINT, h);
		signal(SIGQUIT, h);
		signal(SIGPIPE, h);
	}
}
