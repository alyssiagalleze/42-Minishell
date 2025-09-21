/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:46:57 by agalleze          #+#    #+#             */
/*   Updated: 2025/09/17 19:26:57 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// manque a gerer "../../.. etc" + "~" + "~/../" + "~/pathname" "chemin relatif"

//TODO : returns are not malloc safe 

char	*find_target(char *path, t_env **env)
{
	char	*target;

	target = NULL;
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
	
	working_dir = getcwd(NULL, 0); //mkdir a; cd a; rm ../a
	if (working_dir == NULL)
		return (perror("getcwd"), 1);
	target = find_target(path, env);
	if (!target)
	{
		free(working_dir);
		return (2); // TODO : what is return if malloc fail ??
	}
	update_variable(env, "OLDPWD", working_dir);
	if (chdir(target) == -1)
		return (free(working_dir), free(target), perror(target), 1);
	free(working_dir);
	working_dir = getcwd(NULL, 0);
	if (working_dir == NULL)
		return (free(working_dir), free(target), perror("getcwd"), 1);
	update_variable(env, "PWD", working_dir);
	free(working_dir);
	free(target);
	return (0);
}
