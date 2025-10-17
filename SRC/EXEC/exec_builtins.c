/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:45:10 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/17 16:20:16 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_fds_s_builtin(t_exec *exec_list, int *fd_in, int *fd_out)
{
	int	i;

	i = 0;
	while (exec_list->command->redir[i])
	{
		if (exec_list->command->redir_kind[i] == IN)
		{
			*fd_in = open_fd_in(i, exec_list, TRUE);
			if (*fd_in == -1)
				return (close_no_exit(*fd_in, *fd_out, 1));
		}
		if (exec_list->command->redir_kind[i] == HDOC)
		{
			printf("heredoc with %s\n", exec_list->command->redir[i]);
		}
		if (exec_list->command->redir_kind[i] == OUT ||
			exec_list->command->redir_kind[i] == OUT_APP)
		{
			*fd_out = open_fd_out(i, exec_list, TRUE);
			if (*fd_out == -1)
				return (close_no_exit(*fd_in, -1, 1));
		}
			i++;
	}
	return (0);
}

int	built_in_redirections(t_exec *exec_list)
{
	int	fd_in;
	int	fd_out;

	fd_in = -1;
	fd_out = -1;
	if (exec_list->command->redir[0])
	{
		open_fds(exec_list, &fd_in, &fd_out, 0);
		if (fd_in == -1 || fd_out == -1)
			return (1);
	}
	if (in_redirections(exec_list))
		if (dup2(fd_in, STDIN_FILENO) == -1)
			return (perror("dup2 fd_in"), 1);
	if (fd_in != -1)
		close(fd_in);
	if (out_redirections(exec_list))
	{
		printf("moi ici ????\n");
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 fd_out"), 1);		
	}
	if (fd_out != -1)
		close(fd_out);
	return (0);
}

int	is_builtin(t_exec *exec_list)
{
	if (str_cmp(exec_list->command->argv[0], "echo", TRUE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "cd", TRUE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "pwd", TRUE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "export", TRUE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "unset", TRUE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "env", TRUE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "exit", TRUE))
		return (TRUE);
	else
		return (FALSE);
}

//TODO pid ???
int	exec_single_builtin(
	t_exec *exec_list, t_env **env, struct s_exec_data *exec_data)
{
	int	exit_status;

	exit_status = 0;
	if (built_in_redirections(exec_list) != 0)
		return (-1);
	exit_status = built_in_exec(exec_list, env);
	if (dup2(exec_data->saved_stds[1], STDOUT_FILENO) == -1)
		return (perror("dup2 restore stdin"), -1);
	return (-exit_status);
}

int	built_in_exec(t_exec *exec_list, t_env **env)
{
	int	exit_status;

	exit_status = 0;
	if (str_cmp(exec_list->command->argv[0], "echo", FALSE) == TRUE)
		exit_status = echo(exec_list->command->argv);
	else if (str_cmp(exec_list->command->argv[0], "cd", FALSE))
		exit_status = cd(exec_list->command->argv, env);
	else if (str_cmp(exec_list->command->argv[0], "pwd", FALSE))
		exit_status = pwd();
	else if (str_cmp(exec_list->command->argv[0], "export", FALSE))
		exit_status = export(exec_list->command->argv, env);
	else if (str_cmp(exec_list->command->argv[0], "unset", FALSE))
		exit_status = unset(exec_list->command->argv, env);
	else if (str_cmp(exec_list->command->argv[0], "env", FALSE))
		exit_status = print_env(env);
	return (exit_status);
}
