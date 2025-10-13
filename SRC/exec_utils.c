/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:41:11 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/13 11:48:19 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char **env_tab)
{
	char	**path_tab;
	int		i;
	
	i = 0;
	while (env_tab[i])
	{
		if (str_ncmp(env_tab[i], "PATH=", 3, FALSE))
		{
			path_tab = ft_split(env_tab[i], ':');
			if (!path_tab)
				return (NULL);
		}
		i++;
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

char	*set_command_path(t_exec *exec_list, char **env)
{
	int		i;
	char	**path_tab;
	char	*cmd_path;
	
	path_tab = get_paths(env);
	if (!path_tab)
		return (NULL);
	cmd_path = NULL;
	i = 0;
	while (path_tab[i])
	{
		cmd_path = append_exec_file(exec_list->command->argv[0], path_tab[i]);
		if (!access(cmd_path, F_OK | X_OK))
			return (exec_cleaner(path_tab, NULL), cmd_path);
		free(cmd_path);	
		i++;
	}
	exec_cleaner(path_tab, NULL);
	cmd_path = append_exec_file(exec_list->command->argv[0], ".");
	if (!access(cmd_path, F_OK | X_OK))
		return (cmd_path);
	free(cmd_path);
	return (NULL);
}

char	*build_env_variable(t_env *node)
{
	char	*var;
	int		len;
	int		i;
	int		j;
	
	len = ft_strlen(node->var_name) + ft_strlen(node->var_value) + 2;
	var = malloc(len);
	if (!var)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len && node->var_name[j])
		var[i++] = node->var_name[j++];
	var[i] = '=';
	i++;
	j = 0;
	while (i < len && node->var_value[j])
		var[i++] = node->var_value[j++];
	var[i] = '\0';
	return (var);
}

char	**transfer_env(t_env **env)
{
	t_env	*current;
	char	**my_env;
	int		lst_len;
	int		i;
	
	lst_len = ft_lstsize(*env);
	i = 0;
	my_env = malloc((lst_len + 1)* (sizeof(char *)));
	if (!my_env)
		return (NULL);
	current = *env;
	while (current && i < lst_len)
	{
		my_env[i] = build_env_variable(current);
		if (!my_env[i])
			return (NULL);
		current = current->next;
		i++;
	}
	my_env[i] = NULL;
	return (my_env);
}


