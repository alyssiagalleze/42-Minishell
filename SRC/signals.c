/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:48:07 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/24 19:32:44 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sa_readline_handler(int	sig)
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

void	init_readline_signals(void)
{
	struct sigaction	sa;
	char				*sa_ptr;
	unsigned long		i;

	i = 0;
	sa_ptr = (char *)&sa;
	while (i < sizeof(sa))
	{
		*(sa_ptr + i) = 0;
		i ++;
	}
	sa.sa_handler = sa_readline_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	sa_heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		close(STDIN_FILENO);
		g_signal = SIGINT;
	}
}

void	init_heredoc_signals(void)
{
	struct sigaction	sa;
	char				*sa_ptr;
	unsigned long		i;

	i = 0;
	sa_ptr = (char *)&sa;
	while (i < sizeof(sa))
	{
		*(sa_ptr + i) = 0;
		i ++;
	}
	sa.sa_handler = sa_heredoc_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

//TODO : normal juste exit ?? Sans clean ??
void	sa_exec_child_handler(int	sig)
{
	printf("HERE ?? MAYBE BAD");
	if (sig == SIGINT)
	{
		exit(sig + 128);
	}
	if (sig == SIGQUIT)
	{
		exit(sig + 128);
	}
}

void	init_exec_father_signals(void)
{
	struct sigaction	sa;
	char				*sa_ptr;
	unsigned long		i;

	i = 0;
	sa_ptr = (char *)&sa;
	while (i < sizeof(sa))
	{
		*(sa_ptr + i) = 0;
		i ++;
	}
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	init_exec_child_signals()
{
	struct sigaction	sa;
	char				*sa_ptr;
	unsigned long		i;

	i = 0;
	sa_ptr = (char *)&sa;
	while (i < sizeof(sa))
	{
		*(sa_ptr + i) = 0;
		i ++;
	}
	sa.sa_handler = sa_exec_child_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
