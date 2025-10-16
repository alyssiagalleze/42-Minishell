/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:46:57 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/15 19:00:24 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// manque a gerer "../../.. etc" + "~" + "~/../" + "~/pathname" "chemin relatif"

//TODO : returns are not malloc safe 

char	*find_target(char *path, t_env **env, int *malloc_fail)
{
	char	*target;

	target = NULL;
	if (path == NULL || str_cmp(path, "~", FALSE))
	{
		target = get_var_value(env, "HOME");
		if (!target || target[0] == 0)
			return (print_err(PROMPT, ": cd: ", "HOME not set\n", NULL), NULL);
		else
			return (target);
	}
	if (str_cmp(path, "-", FALSE))
	{
		target = get_var_value(env, "OLDPWD");
		if (target == NULL)
			return (print_err(PROMPT, ": cd: ", "OLDPWD not set\n", NULL), 
				NULL);
		else
			return (target);
	}
	target = ft_strdup(path);
	if (!target)
		return (*malloc_fail = 1, NULL);
	return (target);
}

int	move_folders(char *target, t_env **env)
{
	char	*working_dir;

	working_dir = getcwd(NULL, 0);
	if (!working_dir)
		return (perror("get working dir"), 1);
	if (update_variable(env, "OLDPWD", working_dir) != 0)
		return (free(working_dir), free(target), print_err(PROMPT,
				": cd: could not update OLDPWD", NULL, NULL), 0);
	if (chdir(target) == -1)
		return (free(working_dir), print_err(PROMPT, ": cd: ",
				NULL, NULL), perror(target), free(target), 0);
	free(working_dir);
	free(target);
	working_dir = getcwd(NULL, 0);
	if (!working_dir)
		return (perror("get working dir"), 1);
	if (update_variable(env, "PWD", working_dir) != 0)
		return (free(working_dir), free(target), print_err(PROMPT
				, ": cd: could not update PWD", NULL, NULL), 0);
	free(working_dir);
	return (1);
}

int	cd(char **args, t_env **env)
{
	char	*target;
	int		malloc_fail;

	target = NULL;
	malloc_fail = 0;
	if (args[2] != NULL)
	{
		print_err(PROMPT, ": cd:", " too many arguments\n", NULL);
		return (1);
	}
	target = find_target(args[1], env, &malloc_fail);
	if (!target && malloc_fail)
		return (2);
	else if (target == NULL)
		return (1);
	if (!move_folders(target, env))
		return (1);
	return (0);
}
