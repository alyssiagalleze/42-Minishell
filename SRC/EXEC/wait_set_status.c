/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_set_status.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:49:05 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/29 13:07:53 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_list_status(int result)
{
	int	exit_status;

	exit_status = 0;
	if (WIFEXITED(result))
		exit_status = WEXITSTATUS(result);
	else if (WIFSIGNALED(result))
		exit_status = WTERMSIG(result) + 128;
	else
		exit_status = result;
	return (exit_status);
}

int	pid_wait_all(int exec_count, pid_t last_pid)
{
	int		status;
	int		exit_status;
	int		result;
	pid_t	pid;

	status = 0;
	result = 0;
	while (exec_count)
	{
		pid = wait(&status);
		if ((WIFSIGNALED(status)))
			g_signal = WTERMSIG(status);
		if (status == SIGINT + 128 || status == SIGQUIT + 128)
			g_signal = status - 128;
		if (pid == last_pid)
			result = status;
		exec_count--;
	}
	exit_status = set_list_status(result);
	init_readline_signals();
	if (last_pid < 0)
		exit_status = -last_pid;
	return (exit_status);
}
