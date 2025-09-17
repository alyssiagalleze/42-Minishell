/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:46:57 by agalleze          #+#    #+#             */
/*   Updated: 2025/09/16 17:25:44 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// manque a gerer "../../.. etc" + "~" + "~/../" + "~/pathname" "chemin relatif"

char	*find_target(char *path, t_env **env)
{
	char	*target;
	char	*working_directory;

	target = NULL;
	working_directory = getcwd(NULL, 0);
	if (!path || path == NULL || str_cmp(path, "~", FALSE))
		return (get_var_value(env, "HOME"));
	if (str_cmp(path, "-", FALSE))
		return (get_var_value(env, "OLDPWD"));
	target = strdup(path);
	return (target);
}

int	cd(char *path, t_env **env)
{
	char	*target;
	char	*working_dir;
	
	target = find_target(path, env);
	working_dir = getcwd(NULL, 0); //mkdir a; cd a; rm ../a
	if (working_dir == NULL)
		return (perror("getcwd"), 1);
	update_variable(env, "OLDPWD", working_dir);
	if (chdir(target) == -1)
		return (perror(target), 1);	
	working_dir = getcwd(NULL, 0);
	if (working_dir == NULL)
		return (perror("getcwd"), 1);
	update_variable(env, "PWD", working_dir);
	free(working_dir);
	free(target);
	return (0);
}
