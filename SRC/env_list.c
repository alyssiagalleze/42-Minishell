/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:18:02 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/14 16:21:08 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

//TODO : malloc fail
t_env	*env_new_node(
	const char *var_name, const char *var_value, int is_exported, int is_local)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->var_name = ft_strdup(var_name);
	if (var_value)
		node->var_value = ft_strdup(var_value);
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

//TODO : est ce qu'il y a du malloc qlq part, si oui err_malloc
//TODO : 5 variables
t_env	*init_env_list(char **env)
{
	t_env	*top;
	char	*var_name;
	char	*var_value;
	char	*sep;
	int		i;

	top = NULL;
	i = 0;
	while (env[i])
	{
		sep = strchr(env[i], '='); //TODO : fonction interdite 
		if (sep)
		{
			*sep = '\0';
			var_name = env[i];
			var_value = sep + 1;
			env_add_node(&top, env_new_node(var_name, var_value, FALSE, FALSE));
			*sep = '=';
		}
		i++;
	}
	return (top);
}

//TODO : ne retourne que 0 ?? Pourquoi retourner un int alors ?
int	update_variable(t_env **env, char *var, char *value)
{
	unset_single(var, env);
	env_add_node(env, env_new_node(var, value, FALSE, FALSE));
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

int	ft_lstsize(t_env *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst -> next;
		i++;
	}
	return (i);
}
