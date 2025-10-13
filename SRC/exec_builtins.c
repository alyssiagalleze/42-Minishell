/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:45:10 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/13 11:54:13 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
