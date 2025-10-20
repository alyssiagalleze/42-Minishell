/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:36:21 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/20 14:35:41 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_for_command(t_exec *exec_list, t_env **my_env
	, int pipefds[2], int saved_stds[2])
{
	char	*path;

	path = NULL;
	if (!is_builtin(exec_list) && exec_list->is_subshell == FALSE)
	{
		path = set_command_path(exec_list, my_env);
		if (!path)
		{
			print_err(exec_list->command->argv[0]
				, ": command not found\n", NULL, NULL);
			close_fds(pipefds, saved_stds);
			exit(127);
		}
	}
	return (path);
}

void	child_exec(
	t_exec *exec_list, t_env **env, int pipefds[2], struct s_exec_data *exec_data)
{
	int		status;
	char 	*path;
	char	**my_env;
	
	my_env = NULL;
	path = get_path_for_command(exec_list, env, pipefds, exec_data->saved_stds);
	if (!path && !is_builtin(exec_list))
	{
		close_fds(pipefds, exec_data->saved_stds);
		exit (127);
	}
	if (redirect_fds(exec_list, pipefds, exec_data) || !exec_list->command->argv[0])
		(close_fds(pipefds, exec_data->saved_stds), exit(0));
	if (pipefds[0] != -1)
		close(pipefds[0]);
	if (pipefds[1] != -1)
		close(pipefds[1]);
	if (is_builtin(exec_list))
	{
		status = built_in_exec(exec_list, env);
		close_fds(pipefds, exec_data->saved_stds);
		exit(status);
	}
	my_env = transfer_env(env);
	if (!my_env)
		return (print_err(PROMPT, ": malloc: ", "environment transfer failed.", NULL), exit(2));
	close_fds(pipefds, exec_data->saved_stds);
	status = execve(path, exec_list->command->argv, my_env);
	perror(exec_list->command->argv[0]);
	free_env_array(my_env);
	exit(status);
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

pid_t	exec_pipeline(t_exec *exec_list, t_env **env, struct s_exec_data *exec_data)
{
	int		pipefds[2];
	pid_t	pid;

	pipefds[0] = -1;
	pipefds[1] = -1;
	if (!exec_list || exec_list->is_subshell || !exec_list->is_command || !exec_list->command)
		return (print_err(PROMPT, "internal: exec_pipeline called for non-command node\n", NULL, NULL), 1);
	if (prepare_pipe(exec_list, pipefds) != 0)
		return (1);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(pipefds));
	if (pid == 0)
	{
		init_exec_child_signals();
		child_exec(exec_list, env, pipefds, exec_data);
	}
	parent_after_fork(exec_list, &exec_data->prev_fd, pipefds);
	return (pid);
}

