/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 13:11:27 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/29 13:12:00 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_variable(t_env **env, char *var, char *value)
{
	t_env	*node;

	node = NULL;
	if (unset_single(var, env))
		return (1);
	node = env_new_node(var, value, FALSE, FALSE);
	if (!node)
		return (print_err("malloc", ": update ", var, " variable."), 2);
	env_add_node(env, node);
	return (0);
}

char	*get_var_value(t_env **env, char *var_name)
{
	t_env	*current;
	char	*var_value;

	current = *env;
	while (current)
	{
		if (str_cmp(current->var_name, var_name, FALSE))
		{
			var_value = ft_strdup(current->var_value);
			if (!var_value)
				return (print_err("malloc", ": get ",
						current->var_name, " : value."), NULL);
			return (var_value);
		}
		current = current->next;
	}
	return (NULL);
}

int	env_size(t_env *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		if (lst->is_local)
			lst = lst->next;
		else
		{
			lst = lst -> next;
			i++;
		}
	}
	return (i);
}
