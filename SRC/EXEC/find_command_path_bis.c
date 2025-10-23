/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_path_bis.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 15:28:28 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/23 15:10:13 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_executable(char *cmd_path, t_exec *exec_list, struct s_exec_data *exec_data)
{
	if (access(cmd_path, X_OK) == -1)
	{
		perror(cmd_path);
		clean_data_close_fds(exec_data, exec_list);
		free(cmd_path);
		exit(126);
	}
}

void	path_variable_missing(char *cmd_name)
{
	print_err(PROMPT, cmd_name, ": No such file or directory\n", NULL);
	free(cmd_name);
	exit(127);
}

void	file_exists(char *cmd_path, t_exec *exec_list, struct s_exec_data *exec_data)
{
	if (access(cmd_path, F_OK) == -1)
	{
		print_err(PROMPT, cmd_path, ": No such file or directory\n", NULL);
		clean_data_close_fds(exec_data, exec_list);
		free(cmd_path);
		exit(127);
	}
}

int	has_slash(char *cmd_name)
{
	int	i;

	i = 0;
	while (cmd_name[i])
	{
		if (cmd_name[i] == '/')
			return (TRUE);
		i ++;
	}
	return (FALSE);
}
