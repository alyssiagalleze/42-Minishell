/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:45:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/14 14:56:11 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_std_fds(int *std_in, int *std_out)
{
	*std_in = dup(STDIN_FILENO);
	if (*std_in == -1)
		return (perror("dup"), 1);
	*std_out = dup(STDOUT_FILENO);
	if (*std_out == -1)
		return (perror("dup"), 1);
	return (0);
}

pid_t	exec_command(t_exec * exec_list, t_env **env, int *prev_fd, int saved_stds[2])
{
	pid_t pid;
	
	if (!exec_list->next && is_builtin(exec_list) == TRUE)
		pid = exec_single_builtin(exec_list, env, saved_stds);
	else if (!exec_list->next && !is_builtin(exec_list))
		return (print_err(exec_list->command->argv[0], ": command not found\n", NULL, NULL), 127);
	else
		pid = exec_pipeline(exec_list, env, prev_fd);
	return (pid);
}

int execute_list(t_exec **exec_list, struct s_data *data)
{
	pid_t last_pid;
	int status;
	int			exec_count;
	static int saved_stds[2];
	static int	prev_fd;

	status = 0;
	prev_fd = -1;
	exec_count = 0;
	if (save_std_fds(&saved_stds[0], &saved_stds[1]) == -1)
		return (1);
	while (*exec_list != NULL)
	{
		if ((*exec_list)->is_subshell)
			last_pid = exec_subshell((*exec_list), data, &prev_fd, saved_stds);
		else if ((*exec_list)->is_command)
			last_pid = exec_command(*exec_list, &data->env, &prev_fd, saved_stds);
		exec_count++;
		clean_exec_node(exec_list);
	}
	if (dup2(saved_stds[0], STDIN_FILENO) == -1)
		return (perror("dup2 restore stdin"), 1);
	status = pid_wait_all(exec_count, last_pid);
	// close_fds(saved_stds);
	return (status);
}

int	pid_wait_all(int exec_count, pid_t last_pid)
{
	int 	status;
	int		exit_status;
	int 	result;
	pid_t	pid;
	
	status = 0;
	while (exec_count)
	{
		exec_count--;
		pid = wait(&status);
		if (pid == last_pid)
			result = status;
	}
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else
		exit_status = status;
	return (exit_status);
}
