/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:43:43 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/24 18:33:31 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sub_pipe_redirect(
	t_exec *exec_list, int pipefds[2], struct s_exec_data *exec_data)
{
	if (exec_data->prev_fd != -1)
	{
		if (dup2(exec_data->prev_fd, STDIN_FILENO) == -1)
			exit(1);
		close(exec_data->prev_fd);
	}
	if (pipefds[1] != -1)
	{
		if (dup2(pipefds[1], STDOUT_FILENO) == -1)
			exit(1);
		close(pipefds[1]);
	}
	if (!exec_list->next)
	{
		if (dup2(exec_data->saved_stds[1], STDOUT_FILENO) == -1)
			exit(1);
		close(exec_data->saved_stds[1]);
	}
	close_fds(pipefds, exec_data->saved_stds);
}

void	launch_subshell(t_exec *exec_list, struct s_data *data)
{
	int	status;

	clean_token_list(&data->token_list_head, FALSE);
	rl_clear_history();
	status = handle_subshell_execution(exec_list, &data->env);
	clean_env(&data->env);
	if (g_signal == SIGINT)
		status = 130;
	if (g_signal == SIGQUIT)
		status = 131;
	exit(status);
}

void	save_prev_fd(t_exec *exec_list, int pipefds[2], int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (exec_list->next)
	{
		*prev_fd = pipefds[0];
		if (pipefds[1] != -1)
			close(pipefds[1]);
	}
	else
	{
		if (pipefds[0] != -1)
			close(pipefds[0]);
		if (pipefds[1] != -1)
			close(pipefds[1]);
		*prev_fd = -1;
	}
}

int	handle_subshell_execution(t_exec *exec_list, t_env **env)
{
	struct s_data	subshell_data;

	subshell_data.env = *env;
	data_reset_pointers(&subshell_data);
	subshell_data.token_list = exec_list->subshell->token_sublist;
	subshell_data.token_list_head = subshell_data.token_list;
	exec_list->subshell->token_sublist = NULL;
	clean_exec_list(&exec_list);
	return (token_list_to_exec(&subshell_data));
}

pid_t	exec_subshell(
	t_exec *exec_list, struct s_data *data, struct s_exec_data *exec_data)
{
	int	pipefds[2];
	int	pid;

	pipefds[0] = -1;
	pipefds[1] = -1;
	if (pipe(pipefds) == -1)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(pipefds));
	if (pid == 0)
	{
		sub_pipe_redirect(exec_list, pipefds, exec_data);
		launch_subshell(exec_list, data);
	}
	save_prev_fd(exec_list, pipefds, &exec_data->prev_fd);
	return (pid);
}
