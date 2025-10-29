/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:36:21 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/29 13:08:22 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_exec(
	t_exec *exec_list, int pipefds[2], struct s_exec_data *exec_data)
{
	int		status;
	char	*path;
	char	**my_env;

	my_env = NULL;
	if (redirect_fds(exec_list, pipefds, exec_data)
		|| !exec_list->command->argv[0])
		(clean_data_close_fds(exec_data, exec_list, 1), exit(0));
	if (is_builtin(exec_list))
		exec_builtin_in_child(exec_list, exec_data);
	else if (!exec_list->is_subshell)
		path = set_command_path(exec_list, exec_data);
	if (transfer_env(&exec_data->env, &my_env))
		return (malloc_exit(exec_list, exec_data));
	close_fds(pipefds, exec_data->saved_stds);
	clean_token_list(&exec_data->token_list, TRUE);
	status = execve(path, exec_list->command->argv, my_env);
	if (status == -1)
	{
		clean_data_close_fds(exec_data, exec_list, 1);
		free(path);
		free_env_array(my_env);
		exit(status);
	}
}

void	parent_after_fork(t_exec *exec_list, int *prev_fd, int pipefds[2])
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

pid_t	exec_pipeline(t_exec *exec_list, struct s_exec_data *exec_data)
{
	pid_t	pid;

	if (!exec_list || exec_list->is_subshell
		|| !exec_list->is_command || !exec_list->command)
		return (print_err(PROMPT, "internal: exec_pipeline called "
				"for non-command node\n", NULL, NULL), 1);
	if (prepare_pipe(exec_list, exec_data->pipefds) != 0)
		return (1);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(exec_data->pipefds));
	if (pid == 0)
	{
		init_exec_child_signals();
		child_exec(exec_list, exec_data->pipefds, exec_data);
	}
	parent_after_fork(exec_list, &exec_data->prev_fd, exec_data->pipefds);
	return (pid);
}
