/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:46:57 by agalleze          #+#    #+#             */
/*   Updated: 2025/09/26 16:57:09 by agalleze         ###   ########.fr       */
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

int	cd(char **args, t_env **env)
{
	char	*target = NULL;
	char	*working_dir;
	
	if (args[2] != NULL)
		return (print_err(PROMPT, ": cd:", " too many arguments", NULL), 1);
	printf("--------------------coucou\n");
	working_dir = getcwd(NULL, 0); //mkdir a; cd a; rm ../a
	if (working_dir == NULL)
		return (perror("getcwd"), 1);
	target = find_target(args[1], env);
	if (!target)
	{
		free(working_dir);
		return (2); // TODO : what is return if malloc fail ??
	}
	update_variable(env, "OLDPWD", working_dir);
	if (chdir(target) == -1)
		return (free(working_dir), perror(target), free(target), 1);
	free(working_dir);
	working_dir = getcwd(NULL, 0);
	if (working_dir == NULL)
		return (free(working_dir), free(target), perror("getcwd"), 1);
	printf("------------------ new dir : %s\n", working_dir);
	update_variable(env, "PWD", working_dir);
	free(working_dir);
	free(target);
	return (0);
}
