/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 15:26:37 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/22 15:37:30 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_for_command(t_exec *exec_list, t_env **my_env, int pipefds[2], int saved_stds[2])
{
	char	*path;

	path = NULL;
	if (!is_builtin(exec_list) && exec_list->is_subshell == FALSE)
	{
		path = set_command_path(exec_list, my_env, pipefds, saved_stds);
		if (!path)
		{
			print_err(exec_list->command->argv[0],
				": command not found\n", NULL, NULL);
			close_fds(pipefds, saved_stds);
			clean_env(my_env);
			exit(127);
		}
	}
	return (path);
}

char	*dup_cmd_arg(t_exec *exec_list, t_env **env, int pipefds[2], int saved_stds[2])
{
	char	*cmd_path;

	cmd_path = NULL;
	file_exists(exec_list->command->argv[0], env, pipefds, saved_stds);
	is_executable(exec_list->command->argv[0], env, pipefds, saved_stds);
	cmd_path = ft_strdup(exec_list->command->argv[0]);
	if (!cmd_path)
		return (print_err(PROMPT, PERR_MALLOC, NULL, NULL), exit(2), NULL);
	return (cmd_path);
}

char	*set_command_path(t_exec *exec_list, t_env **env, int pipefds[2], int saved_stds[2])
{
	int		i;
	char	**path_tab;
	char	*cmd_path;

	cmd_path = NULL;
	if (exec_list->command->argv[0] && has_slash(exec_list->command->argv[0]))
		return (dup_cmd_arg(exec_list, env, pipefds, saved_stds));
	path_tab = get_paths(env);
	if (!path_tab)
		return (path_variable_missing(exec_list->command->argv[0]), NULL);
	i = 0;
	while (path_tab[i])
	{
		cmd_path = append_exec_file(exec_list->command->argv[0], path_tab[i]);
		if (!cmd_path)
			return (exec_cleaner(path_tab, NULL), NULL);
		if (!access(cmd_path, F_OK))
			return (is_executable(cmd_path, env, pipefds, saved_stds), exec_cleaner(path_tab, NULL), cmd_path);
		free(cmd_path);
		i++;
	}
	cmd_path = append_exec_file(exec_list->command->argv[0], ".");
	if (!access(cmd_path, F_OK))
		return (is_executable(cmd_path, env, pipefds, saved_stds), exec_cleaner(path_tab, NULL), cmd_path);
	return (exec_cleaner(path_tab, NULL), free(cmd_path), NULL);
}

char	**get_paths(t_env **env)
{
	char	**path_tab;
	t_env	*current;

	current = *env;
	path_tab = NULL;
	while (current)
	{
		if (str_ncmp(current->var_name, "PATH=", 3, FALSE))
		{
			path_tab = ft_split(current->var_value, ':');
			if (!path_tab)
				return (NULL);
		}
		current = current->next;
	}
	return (path_tab);
}

char	*append_exec_file(char *cmd_name, char *path)
{
	char	*tmp;
	char	*cmd_path;

	tmp = NULL;
	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	cmd_path = ft_strjoin(tmp, cmd_name);
	if (!cmd_path)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (cmd_path);
}
