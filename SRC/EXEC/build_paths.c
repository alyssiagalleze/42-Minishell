/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_paths.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:44:39 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/29 13:05:37 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(t_env **env, int *err_malloc)
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
				return (*err_malloc = 1, NULL);
			break ;
		}
		current = current->next;
	}
	return (path_tab);
}

char	*append_exec_file(char *cmd_name, char *path, int *err_malloc)
{
	char	*tmp;
	char	*cmd_path;

	(void)cmd_name;
	tmp = NULL;
	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (*err_malloc = 1, NULL);
	cmd_path = ft_strjoin(tmp, cmd_name);
	if (!cmd_path)
	{
		free(tmp);
		return (*err_malloc = 1, NULL);
	}
	free(tmp);
	return (cmd_path);
}

int	is_empty_at_start(struct s_exec_data *exec_data)
{
	t_env	*current;

	current = exec_data->env;
	while (current)
	{
		if (str_ncmp(current->var_name, "PATH=", 3, FALSE))
		{
			if (current->var_value[0] == ':')
				return (TRUE);
			else
				return (FALSE);
		}
		current = current->next;
	}
	return (FALSE);
}

int	is_empty_at_end(struct s_exec_data *exec_data)
{
	t_env	*current;

	current = exec_data->env;
	while (current)
	{
		if (str_ncmp(current->var_name, "PATH=", 3, FALSE))
		{
			if (current->var_value[ft_strlen(current->var_value) - 1] == ':')
				return (TRUE);
			else
				return (FALSE);
		}
		current = current->next;
	}
	return (FALSE);
}
