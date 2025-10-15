/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:45:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/15 19:28:45 by tfiette          ###   ########.fr       */
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
			pid = exec_assign_var(exec_list, env);
	}
	else
		pid = exec_pipeline(exec_list, env, prev_fd);
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
	init_exec_father_signals();
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
	// close(saved_stds[0]);
	// close(saved_stds[1]);
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
	return (exit_status);
}
