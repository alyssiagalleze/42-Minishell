/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:45:10 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/14 16:09:43 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in_redirections( t_exec *exec_list)
{
	int fd_in;
	int fd_out;
	
	fd_in = -1;
	fd_out = -1;
	if (exec_list->command->redir[0])
		open_fds(exec_list, &fd_in, &fd_out);
	if (in_redirections(exec_list))
		if (dup2(fd_in, STDIN_FILENO) == -1)
			return (perror("dup2 fd_in"));
	if (out_redirections(exec_list))
	{
		printf("moi ici ????\n");
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 fd_out"));		
	}
}

int	is_builtin(t_exec *exec_list)
{
	if (str_cmp(exec_list->command->argv[0], "echo", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "cd", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "pwd", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "export", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "unset", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "env", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "exit", FALSE))
		return (TRUE);
	else
		return (FALSE);
}

//TODO pid ???
int exec_single_builtin(t_exec *exec_list, t_env **env, int saved_stds[2])
{
	int exit_status;

	exit_status = 0;
	built_in_redirections(exec_list);
	exit_status = built_in_exec(exec_list, env);
	if (dup2(saved_stds[1], STDOUT_FILENO) == -1)
		return (perror("dup2 restore stdin"), 1);
	return (exit_status);
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
