/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:45:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/23 13:35:13 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO: clean malloc
//TODO: exit close etc
pid_t	exec_assign_var(t_exec *exec_list, t_env **env)
{
	t_env	*node;
	int		i;
	char	*var_value;
	char	*var_name;

	var_name = exec_list->command->argv[0];
	i = is_char_in_string('=', var_name, FALSE, TRUE);
	var_name[i] = '\0';
	if (!is_string_valid_var(var_name))
	{
		return (-1);
	}
	var_value = ft_strdup(var_name + i + 1);
	if (!var_value)
		return (print_err(PROMPT, PERR_ASSIGN, var_name, NULL), -2);
	node = var_exists(env, var_name);
	if (!node)
	{
		node = env_new_node(var_name, var_value, FALSE, TRUE);
		free(var_value);
		if (!node)
			return (print_err(PROMPT, PERR_ASSIGN, var_name, NULL), -2);
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
	return (0);
}

int	is_var(t_exec *exec_list)
{
	return (exec_list->command->is_var);
}

//TODO: si tu veux fais ca joli
pid_t	exec_command(t_exec *exec_list, t_env **env, struct s_exec_data *exec_data)
{	
	pid_t		pid;

	if (exec_data->is_pipe == 0)
		exec_data->is_pipe = exec_list->next && exec_list->next->is_command;
	if (!exec_data->is_pipe
		&& (is_builtin(exec_list) || exec_list->command->is_var))
	{
		if (is_builtin(exec_list))
			pid = exec_single_builtin(exec_list, env, exec_data);
		if (is_var(exec_list))
			pid = exec_assign_var(exec_list, env);
	}
	else
		pid = exec_pipeline(exec_list, env, exec_data);
	if (!exec_list->next)
		exec_data->is_pipe = 0;
	return (pid);
}
void	init_exec_data(struct s_exec_data *exec_data)
{
	exec_data->prev_fd = -1;
	exec_data->exec_count = 0;
	exec_data->saved_stds[0] = -1;
	exec_data->saved_stds[1] = -1;
	exec_data->is_pipe = 0;
	exec_data->pipefds[0] = -1;
	exec_data->pipefds[1] = -1;
}


int	execute_list(t_exec **exec_list, struct s_data *data)
{
	struct s_exec_data	exec_data;

	init_exec_father_signals();
	init_exec_data(&exec_data);

	if (save_std_fds(&exec_data.saved_stds[0], &exec_data.saved_stds[1]) == -1)
		return (1);
	while (*exec_list != NULL)
	{
		// printf("Executing new node, cmd : %s, arg : %s\n", (*exec_list)->command->argv[0], (*exec_list)->command->argv[1]);
		if ((*exec_list)->is_subshell)
			exec_data.last_pid = exec_subshell((*exec_list), data, &exec_data);
		else if ((*exec_list)->is_command)
			exec_data.last_pid = exec_command(*exec_list, &data->env, &exec_data);
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
		// printf("waiting for a process !\n");
		// signal(SIGINT, SIG_DFL);
		pid = wait(&status);
		if ((WIFSIGNALED(status)))
			g_signal = WTERMSIG(status);
		if (status == SIGINT + 128 || status == SIGQUIT + 128)
			g_signal = status - 128;
		if (pid == last_pid)
			result = status;
		exec_count--;
	}

	if (WIFEXITED(result))
		exit_status = WEXITSTATUS(result);
	else if (WIFSIGNALED(result))
		exit_status = WTERMSIG(result) + 128;
	else
		exit_status = result;
	init_readline_signals();
	if (last_pid < 0)
		exit_status = -last_pid;
	return (exit_status);
}
