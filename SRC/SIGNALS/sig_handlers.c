/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 20:07:45 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 14:41:04 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sa_readline_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_signal = SIGINT;
	}
}

void	sa_heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		close(STDIN_FILENO);
		g_signal = SIGINT;
	}
}

void	sa_exec_child_handler(int sig)
{
	if (sig == SIGINT)
	{
		exit(sig + 128);
	}
	if (sig == SIGQUIT)
	{
		exit(sig + 128);
	}
}
