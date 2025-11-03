/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 10:52:45 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 12:49:00 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prev_command_check_signal(int *status)
{
	if (g_signal == SIGINT || *status == SIGINT + 128)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	if (g_signal == SIGQUIT || *status == SIGQUIT + 128)
	{
		write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

int	heredoc_check_signal(struct s_data *data, int *status)
{
	if (g_signal == SIGINT)
	{
		cleaner(NULL, NULL, &data->token_list_head);
		*status = SIGINT + 128;
		g_signal = 0;
		return (TRUE);
	}
	return (FALSE);
}
