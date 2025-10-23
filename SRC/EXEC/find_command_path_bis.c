/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_path_bis.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 15:28:28 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/23 11:40:42 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_executable(char *cmd_path, t_env **env, int pipefds[2], int saved_stds[2])
{
	if (access(cmd_path, X_OK) == -1)
	{
		perror(cmd_path);
		close_fds(pipefds, saved_stds);
		clean_env(env);
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

void	file_exists(char *cmd_path, t_env **env, int pipefds[2], int saved_stds[2])
{
	if (access(cmd_path, F_OK) == -1)
	{
		print_err(PROMPT, cmd_path, ": No such file or directory\n", NULL);
		close_fds(pipefds, saved_stds);
		clean_env(env);
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
