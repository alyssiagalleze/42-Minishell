/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:30:56 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/28 16:17:17 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_exported(t_env **env)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (current->is_local)
		{
			current = current->next;
			continue ;
		}
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(current->var_name, 1);
		if (!current->is_exported)
		{
			write(1, "=\"", 2);
			ft_putstr_fd(current->var_value, 1);
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		current = current->next;
	}
	return (0);
}

t_env	*create_exported_var(char *argument, t_env **env, int *malloc_fail)
{
	t_env	*new;
	char	*sep;

	new = NULL;
	sep = NULL;
	sep = ft_strchr(argument, '=');
	if (!sep)
	{
		if (!is_string_valid_var(argument) || var_exists(env, argument))
			return (NULL);
		new = env_new_node(argument, NULL, TRUE, FALSE);
		if (!new)
			return (*malloc_fail = 1, NULL);
	}
	*sep = '\0';
	if (!is_string_valid_var(argument))
		return (NULL);
	if (var_exists(env, argument))
		unset_single(argument, env);
	new = env_new_node(argument, sep + 1, FALSE, FALSE);
	if (!new)
		return (*malloc_fail = 1, NULL);
	return (new);
}

int	node_create_fail(t_env **env, t_env *new, char *argument, int malloc_fail)
{
	if (!new && var_exists(env, argument))
		return (0);
	if (!new && malloc_fail)
	{
		rl_clear_history();
		return (2);
	}
	return (1);
}

int	export(char **args, t_env **env)
{
	int		i;
	int		malloc_fail;
	t_env	*new_node;

	i = 1;
	malloc_fail = 0;
	if (args[i] == NULL)
		return (print_exported(env), 0);
	while (args[i])
	{
		if (args[i][0] != '\0')
		{
			new_node = create_exported_var(args[i], env, &malloc_fail);
			if (!new_node && malloc_fail)
				return (node_create_fail(env, new_node, args[i], malloc_fail));
			if (new_node)
				env_add_node(env, new_node);
		}
		else
			print_err(PROMPT, ": export:", "\'\'", ": not a valid identifier\n");
		i++;
	}
	return (0);
}
