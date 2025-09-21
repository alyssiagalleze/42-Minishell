/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:30:56 by agalleze          #+#    #+#             */
/*   Updated: 2025/09/17 19:03:23 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	var_exists(t_env **env, char *name)
{
	t_env *current;

	current = *env;
	while (current)
	{
		if (str_cmp(current->var_name, name, FALSE))
		return (TRUE);
		current = current->next;
	}
	return (FALSE);
}

int	is_string_valid(char *str)
{
	while (str && *str)
	{
		if (!(*str >= 'a' && *str <= 'z') && !(*str >= 'A' && *str < 'Z') && *str != '_')
		{
			str++ ;
			*str = '\0';
			str--;
			ft_putstr_fd(str, 2);
			return (ft_putstr_fd(": not a valid identifier\n", 2), FALSE);
		}	
		str++;
	}
	return (TRUE);
}

int	print_exported(t_env **env)
{
    t_env *current;

    current = *env;
    while (current)
    {
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(current->var_name, 1);
		if (!current->is_exported)
		{
			write(1, "=", 1);
			ft_putstr_fd(current->var_value, 1);
		}
		write(1, "\n", 1);
        current = current->next;
    }
	return (0);
}

int	export(char **args, t_env **env)
{
	int		i;
	char	*sep;
	t_env	*new_node;
	
	i = 0;

	if (!args || args[i] == NULL)
	{
		print_exported(env);
		return (0);
	}
	while (args[i]) // needs a tab of strings
	{
		sep = strchr(args[i], '='); //ILLEGAL
		if (!sep)
		{
			if (!is_string_valid(args[i]))
				return (1);
			// look for variable in env, if not present add it with export toggle on and value null
			if (var_exists(env, args[i]))
				return (0);
			new_node = env_new_node(args[i], NULL, TRUE);
		}
		else
		{
			*sep = '\0';
			if (!is_string_valid(args[i]))
			{
				errno = 1;
				return (1);
			}
			new_node = env_new_node(args[i], sep + 1, FALSE);
			// free(sep);
			// sep = NULL;
		}
		env_add_node(env, new_node);
		i++;
	}
	return (0);
}
