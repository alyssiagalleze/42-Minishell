/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:45:10 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/22 18:45:03 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	built_in_redirections(t_exec *exec_list, struct s_exec_data *exec_data)
{
	int	fd_in;
	int	fd_out;

	fd_in = -1;
	fd_out = -1;
	if (exec_list->command->redir[0])
	{
		open_fds(exec_list, &fd_in, &fd_out, exec_data);
		if (fd_in == -1 && fd_out == -1)
			return (1);
	}
	if (in_redirections(exec_list))
		if (dup2(fd_in, STDIN_FILENO) == -1)
			return (perror("dup2 fd_in"), 1);
	if (fd_in != -1)
		close(fd_in);
	if (out_redirections(exec_list))
	{
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

int	parse_exit(char **args)
{
	int	status;

	status = 0;
	if (is_only_digit(args[1]))
	{
		print_err(PROMPT, ": exit: ", args[1], ": numeric argument required");
		exit(2);
	}
	if (args[2] != NULL)
	{
		print_err(PROMPT, ": exit: too many arguments", NULL, NULL);
		exit(2);
	}
	status = ft_atoi(args[1]);
	return (status);
}

pid_t	exec_single_builtin(
	t_exec *exec_list, t_env **env, struct s_exec_data *exec_data)
{
	printf("Executing single built-in: %s\n", exec_list->command->argv[0]);
	pid_t	exit_status;

	exit_status = 0;
	if (built_in_redirections(exec_list, exec_data) != 0)
		return (-1);
	exit_status = built_in_exec(exec_list, env, exec_data);
	if (dup2(exec_data->saved_stds[1], STDOUT_FILENO) == -1)
		return (perror("dup2 restore stdin"), -1);
	return (-exit_status);
}

pid_t	built_in_exec(t_exec *exec_list, t_env **env, struct s_exec_data *exec_data)
{
	pid_t	exit_status;

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
	else if (exec_data->is_pipe && str_cmp(exec_list->command->argv[0], "exit", FALSE))
	{
		printf("+++++++++exit ????env size = %d\n", env_size(*env));
		my_exit_builtin(exec_list->command->argv, exec_data, NULL, NULL);
	}
	else if (!exec_data->is_pipe && str_cmp(exec_list->command->argv[0], "exit", FALSE))
	{
		printf("exit !!!!!!!\n");
		my_exit_builtin(exec_list->command->argv, exec_data, NULL, NULL);
	}
	return (exit_status);
}
