/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sigs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:48:07 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/28 17:46:54 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	init_exec_child_signals(void)
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
