/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 15:26:37 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 14:40:58 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*browse_current_dir(
	t_exec *exec_list, struct s_exec_data *exec_data, int *first_errno)
{
	char	*cmd_path;
	int		err_malloc;

	cmd_path = NULL;
	err_malloc = 0;
	if (!has_slash(exec_list->command->argv[0]))
	{
		cmd_path = append_exec_file(
				exec_list->command->argv[0], "./", &err_malloc);
		if (!cmd_path && err_malloc)
			(malloc_exit(exec_list, exec_data));
	}
	else
	{
		cmd_path = ft_strdup(exec_list->command->argv[0]);
		if (!cmd_path)
			(malloc_exit(exec_list, exec_data));
	}
	cmd_path = check_access(cmd_path, first_errno);
	return (cmd_path);
}

char	*browse_current_path(t_exec *exec_list,
		struct s_exec_data *exec_data, int *first_errno, char *path)
{
	char	*cmd_path;
	int		err_malloc;

	err_malloc = 0;
	cmd_path = NULL;
	if (*path)
		cmd_path = append_exec_file(
				exec_list->command->argv[0], path, &err_malloc);
	else
		cmd_path = append_exec_file(
				exec_list->command->argv[0], "./", &err_malloc);
	if (!cmd_path && err_malloc)
		(free_env_array(exec_data->path_tab)
			, malloc_exit(exec_list, exec_data));
	cmd_path = check_access(cmd_path, first_errno);
	return (cmd_path);
}

char	*browse_paths(t_exec *exec_list,
	char **path_tab, int *first_errno, struct s_exec_data *exec_data)
{
	int		i;
	char	*cmd_path;
	int		err_malloc;

	i = 0;
	cmd_path = NULL;
	err_malloc = 0;
	if (is_empty_at_start(exec_data))
		cmd_path = browse_current_dir(exec_list, exec_data, first_errno);
	if (cmd_path)
		return (cmd_path);
	while (path_tab[i])
	{
		cmd_path = browse_current_path(
				exec_list, exec_data, first_errno, path_tab[i]);
		if (cmd_path)
			return (cmd_path);
		i++;
	}
	if (is_empty_at_end(exec_data))
		cmd_path = browse_current_dir(exec_list, exec_data, first_errno);
	return (cmd_path);
}

void	error_exit(
	t_exec *exec_list, struct s_exec_data *exec_data, int first_errno)
{
	if (first_errno != 0)
	{
		print_err(
			PROMPT, exec_list->command->argv[0], ": ", strerror(first_errno));
		write(2, "\n", 1);
		free_env_array(exec_data->path_tab);
		clean_data_close_fds(exec_data, exec_list, 1);
		if (first_errno == EACCES)
			exit(126);
		else
			exit(127);
	}
	else
	{
		print_err(
			PROMPT, exec_list->command->argv[0], ": ", "command not found\n");
		clean_data_close_fds(exec_data, exec_list, 1);
		free_env_array(exec_data->path_tab);
		exit(127);
	}
	exit(1);
}

char	*set_command_path(t_exec *exec_list, struct s_exec_data *exec_data)
{
	int		err_malloc;
	int		first_err;
	char	*cmd_path;

	cmd_path = NULL;
	err_malloc = 0;
	first_err = 0;
	if (exec_list->command->argv[0] && has_slash(exec_list->command->argv[0]))
		cmd_path = browse_current_dir(exec_list, exec_data, &first_err);
	if (cmd_path)
		return (cmd_path);
	exec_data->path_tab = get_paths(&exec_data->env, &err_malloc);
	if (!exec_data->path_tab && err_malloc)
		return (malloc_exit(exec_list, exec_data), NULL);
	else if (!exec_data->path_tab)
		cmd_path = browse_current_dir(exec_list, exec_data, &first_err);
	if (cmd_path)
		return (cmd_path);
	if (exec_data->path_tab)
		cmd_path = browse_paths(
				exec_list, exec_data->path_tab, &first_err, exec_data);
	if (cmd_path)
		return (cmd_path);
	return (error_exit(exec_list, exec_data, first_err), NULL);
}
