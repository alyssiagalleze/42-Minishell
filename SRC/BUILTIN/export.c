/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 18:30:56 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/17 18:11:27 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*var_exists(t_env **env, char *name)
{
	t_env *current;

	current = *env;
	while (current)
	{
		if (str_cmp(current->var_name, name, FALSE))
			break;
		current = current->next;
	}
	return (current);
}

int	is_string_valid_var(char *str)
{
	int	is_valid;

	is_valid = TRUE;
	if (!str || (!(*str >= 'a' && *str <= 'z') && !(*str >= 'A' && *str < 'Z') && *str != '_'))
		is_valid = FALSE;
	while (is_valid && str && *str)
	{
		if (!(*str >= 'a' && *str <= 'z') && !(*str >= 'A' && *str < 'Z') && *str != '_')
		{
			is_valid = FALSE;
			break ;
		}	
		str++;
	}
	if (!is_valid)
	{
		str++ ;
		*str = '\0';
		str--;
		print_err(PROMPT, str, ": not a valid identifier\n", NULL);
		return (FALSE);
	}
	return (TRUE);
}

int	print_exported(t_env **env)
{
    t_env *current;

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
			write(1, "=", 1);
			ft_putstr_fd(current->var_value, 1);
		}
		write(1, "\n", 1);
        current = current->next;
    }
	return (0);
}

//TODO: malloc fail
int	export(char **args, t_env **env)
{
	int		i;
	char	*sep;
	t_env	*new_node;
	
	i = 1;

	//printf("coucou\n");
	if (/* !args ||  */args[i] == NULL)
	{
		print_exported(env);
		return (0);
	}
	while (args[i]) // needs a tab of strings
	{
		sep = strchr(args[i], '='); //ILLEGAL
		if (!sep)
		{
			if (!is_string_valid_var(args[i]))
				return (1);
			// look for variable in env, if not present add it with export toggle on and value null
			if (var_exists(env, args[i]))
				return (0);
			new_node = env_new_node(args[i], NULL, TRUE, FALSE);
		}
		else
		{
			*sep = '\0';
			if (!is_string_valid_var(args[i]))
			{
				errno = 1;
				return (1);
			}
			if (var_exists(env, args[i]))
			{
				//printf("var : %s exists\n", args[i]);
				unset_single(args[i], env);
			}
			new_node = env_new_node(args[i], sep + 1, FALSE, FALSE);
			// free(sep);
			// sep = NULL;
		}
		env_add_node(env, new_node);
		i++;
	}
	return (0);
}
