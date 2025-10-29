/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:18:02 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/29 13:11:56 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_new_node(
	const char *var_name, const char *var_value, int is_exported, int is_local)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->var_name = ft_strdup(var_name);
	if (!node->var_name)
		return (free(node), NULL);
	if (var_value)
	{
		node->var_value = ft_strdup(var_value);
		if (!node->var_value)
		{
			free(node->var_name);
			return (free(node), NULL);
		}
	}
	else
		node->var_value = NULL;
	node->is_exported = is_exported;
	node->is_local = is_local;
	node->next = NULL;
	return (node);
}

void	env_add_node(t_env **top, t_env *node)
{
	t_env	*tmp;

	if (!*top)
		*top = node;
	else
	{
		tmp = *top;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;
	}
}

t_env	*create_env_node(char *env_variable)
{
	t_env	*node;
	char	*sep;
	char	*var_name;
	char	*var_value;

	sep = ft_strchr(env_variable, '=');
	if (sep)
	{
		*sep = '\0';
		var_name = env_variable;
		var_value = sep + 1;
		node = env_new_node(var_name, var_value, FALSE, FALSE);
		*sep = '=';
		return (node);
	}
	else
	{
		var_name = env_variable;
		var_value = NULL;
		node = env_new_node(var_name, var_value, FALSE, FALSE);
		return (node);
	}
	return (NULL);
}

int	init_env_list(char **env, t_env **env_list)
{
	t_env	*top;
	t_env	*new_node;
	int		i;

	top = NULL;
	i = 0;
	while (env[i])
	{
		new_node = create_env_node(env[i]);
		if (!new_node)
		{
			clean_env(&top);
			*env_list = NULL;
			return (
				print_err("malloc", ": init ", "env list", "."), ERR_MALLOC);
		}
		env_add_node(&top, new_node);
		i++;
	}
	*env_list = top;
	return (ERR_SUCCESS);
}
