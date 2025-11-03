/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:45:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 12:10:55 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe(t_exec *exec_list)
{
	return (exec_list->next && exec_list->next->is_command);
}

pid_t	exec_command(
	t_exec *exec_list, t_env **env, struct s_exec_data *exec_data)
{	
	pid_t		pid;

	if (exec_data->is_pipe == 0)
			exec_data->is_pipe = is_pipe(exec_list);
	if (!exec_data->is_pipe
		&& (is_builtin(exec_list) || exec_list->command->is_var))
	{
		if (is_builtin(exec_list))
			pid = exec_single_builtin(exec_list, exec_data);
		if (is_var(exec_list))
			pid = exec_assign_var(exec_list, env);
	}
	else
		pid = exec_pipeline(exec_list, exec_data);
	if (!exec_list->next)
		exec_data->is_pipe = 0;
	return (pid);
}

void	init_exec_data(struct s_exec_data *exec_data, struct s_data *data)
{
	exec_data->prev_fd = -1;
	exec_data->exec_count = 0;
	exec_data->saved_stds[0] = -1;
	exec_data->saved_stds[1] = -1;
	exec_data->is_pipe = 0;
	exec_data->pipefds[0] = -1;
	exec_data->pipefds[1] = -1;
	exec_data->path_tab = NULL;
	exec_data->env = data->env;
	exec_data->token_list = data->token_list_head;
}

int	execute_list(t_exec **exec_list, struct s_data *data)
{
	struct s_exec_data	exec_data;

	init_exec_father_signals();
	init_exec_data(&exec_data, data);
	if (save_std_fds(&exec_data.saved_stds[0], &exec_data.saved_stds[1]) == -1)
		return (1);
	while (*exec_list != NULL)
	{
		if ((*exec_list)->is_subshell)
			exec_data.last_pid = exec_subshell((*exec_list), data, &exec_data);
		else if ((*exec_list)->is_command)
			exec_data.last_pid = exec_command(
					*exec_list, &data->env, &exec_data);
		if (exec_data.last_pid >= 0)
			exec_data.exec_count++;
		clean_exec_node(exec_list);
	}
	restore_std_fds(exec_data.saved_stds);
	if (exec_data.prev_fd != -1)
		close(exec_data.prev_fd);
	if (exec_data.last_pid == -7)
		return (-7);
	return (pid_wait_all(exec_data.exec_count, exec_data.last_pid));
}
