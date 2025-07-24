/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:15:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/24 08:56:13 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "handler.h"

volatile sig_atomic_t	g_sig = 0;

void	sigint_handler(int32_t sig)
{
	g_sig = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_when_child_handler(int32_t sig)
{
	g_sig = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_when_child_handler(int32_t sig)
{
	g_sig = sig;
	write(2, "Quit (core dumped)\n", 19);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	hd_quit_handler(int32_t sig)
{
	g_sig = sig;
	rl_done = 1;
}

void	sig_init(t_sig_handle action)
{
	void	(*h)(int);

	if (action == SIGH_MAIN)
	{
		signal(SIGINT, &sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
	}
	else if (action == SIGH_RUNNING_CH)
	{
		signal(SIGINT, &sigint_when_child_handler);
		signal(SIGQUIT, &sigquit_when_child_handler);
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
