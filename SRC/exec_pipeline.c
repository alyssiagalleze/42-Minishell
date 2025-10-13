/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:36:21 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/13 12:45:31 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_path_for_command(t_exec *exec_list, char **my_env, int pipefds[2])
{
	char *path;

	path = NULL;
	if (!is_builtin(exec_list) && exec_list->is_subshell == FALSE)
	{
		path = set_command_path(exec_list, my_env);
		if (!path)
		{
			print_err(exec_list->command->argv[0], ": command not found\n", NULL, NULL);
			if (pipefds[0] != -1)
				close(pipefds[0]);
			if (pipefds[1] != -1)
				close(pipefds[1]);
			return (NULL);
		}
	}
	return (path);
}

void child_exec(t_exec *exec_list, char *path, t_env **env, int pipefds[2], int prev_fd)
{
	int		status;
	char	**my_env;

	if (redirect_fds(exec_list, pipefds, prev_fd))
		exit(1);
	if (pipefds[0] != -1)
		close(pipefds[0]);
	if (pipefds[1] != -1)
		close(pipefds[1]);
	if (is_builtin(exec_list))
	{
		status = built_in_exec(exec_list, env);
		exit(status);
	}
	my_env = transfer_env(env);
	status = execve(path, exec_list->command->argv, my_env);
	perror(exec_list->command->argv[0]);
	free_env_array(my_env);
	exit(85);
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

int exec_pipeline(t_exec *exec_list, t_pid_list **pids, t_env **env, int *prev_fd)
{
	printf("-> in exec_pipeline\n");
	int		pipefds[2];
	pid_t	pid;
	char	*path;
	char	**my_env;

	if (!exec_list || exec_list->is_subshell || !exec_list->is_command || !exec_list->command)
		return (print_err(PROMPT, "internal: exec_pipeline called for non-command node\n", NULL, NULL), 1);
	if (prepare_env_and_pipe(exec_list, env, &my_env, pipefds) != 0)
		return (1);
	path = get_path_for_command(exec_list, my_env, pipefds);
	if (!path && !is_builtin(exec_list))
		return (free_env_array(my_env), 127);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(pipefds, my_env));
	if (pid == 0)
		child_exec(exec_list, path, env, pipefds, *prev_fd);
	parent_after_fork(exec_list, prev_fd, pipefds);
	pid_add_back(pids, pid);
	free_env_array(my_env);
	return (0);
}
