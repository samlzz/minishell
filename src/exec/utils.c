/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:19:40 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/25 10:42:21 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <unistd.h>

void	ft_close_pipe(int32_t fds[2])
{
	if (fds && fds[0] != STDIN_FILENO && fds[0] != -1)
		close(fds[0]);
	if (fds && fds[1] != STDOUT_FILENO && fds[1] != -1)
		close(fds[1]);
}

void	ft_close_all(int32_t fds[2])
{
	ft_close_pipe(fds);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}
