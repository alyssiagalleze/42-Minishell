/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:45:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/15 14:31:53 by agalleze         ###   ########.fr       */
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

//TODO: clean malloc
//TODO: exit close etc
pid_t	exec_assign_var(t_exec *exec_list, t_env **env, int	saved_stds[2])
{
	t_env	*node;
	int		i;
	char	*var_value;
	char	*var_name;

	var_name = exec_list->command->argv[0];
	i = is_char_in_string('=', var_name, FALSE, TRUE);
	var_name[i] = '\0';
	var_value = ft_strdup(var_name + i + 1);
	if (!*var_value)
	{
		//MALLOC FAIL ?? IL SE PASSE QUOI SI *var_lue = \0 ?
	}
	node = var_exists(env, var_name);
	if (!node)
	{
		node = env_new_node(var_name, var_value, FALSE, TRUE);
		free(var_value);
		//TODO: MALLOC FAIL
		env_add_node(env, node);
	}
	else
	{
		if (node->var_value)
			free(node->var_value);
		node->var_value = var_value;
		node->is_exported = FALSE;
		node->is_local = TRUE;
	}
	// if (dup2(saved_stds[1], STDOUT_FILENO) == -1)
	// 	return (perror("dup2 restore stdin"), 1);
	// node = var_exists(env, )
	(void)saved_stds;
	return (0);
}

//TODO: si tu veux fais ca joli
pid_t	exec_command(t_exec *exec_list, t_env **env, int *prev_fd, int saved_stds[2])
{
	pid_t pid;
	
	if (!exec_list->next
		&& (is_builtin(exec_list) || exec_list->command->is_var))
	{
		if (is_builtin(exec_list))
			pid = exec_single_builtin(exec_list, env, saved_stds);
		if (exec_list->command->is_var)
			pid = exec_assign_var(exec_list, env, saved_stds);
	}
	else
		pid = exec_pipeline(exec_list, env, prev_fd, saved_stds);
	return (pid);
}

int execute_list(t_exec **exec_list, struct s_data *data)
{
	pid_t 		last_pid;
	int			exec_count;
	static int 	saved_stds[2];
	static int	prev_fd;

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
		if (last_pid >= 0)
			exec_count++;
		clean_exec_node(exec_list);
	}
	if (dup2(saved_stds[0], STDIN_FILENO) == -1)
		return (perror("dup2 restore stdin"), 1);
	if (dup2(saved_stds[1], STDOUT_FILENO) == -1)
		return (perror("dup2 restore stdin"), 1);
	close(saved_stds[0]);
	close(saved_stds[1]);
	if (prev_fd != -1)
		close(prev_fd);
	// close_fds(saved_stds);
	return (pid_wait_all(exec_count, last_pid));
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
		pid = wait(&status);
		if (pid == last_pid)
			result = status;
		exec_count--;
	}
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else
		exit_status = status;
	return (exit_status);
}
