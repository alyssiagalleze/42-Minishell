/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   export.c										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agalleze <agalleze@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/09/16 18:30:56 by agalleze		  #+#	#+#			 */
/*   Updated: 2025/10/20 16:37:26 by agalleze		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

t_env	*var_exists(t_env **env, char *name)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (str_cmp(current->var_name, name, FALSE))
			break ;
		current = current->next;
	}
	return (current);
}

int	is_string_valid(char *str)
{
	while (str && *str)
	{
		if (!(*str >= 'a' && *str <= 'z')
			&& !(*str >= 'A' && *str < 'Z') && *str != '_')
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
			write(1, "=", 1);
			ft_putstr_fd(current->var_value, 1);
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

	sep = NULL;
	sep = ft_strchr(argument, '=');
	if (!sep)
	{
		if (!is_string_valid(argument) || var_exists(env, argument))
			return (NULL);
		new = env_new_node(argument, NULL, TRUE, FALSE);
		if (!new)
			return (*malloc_fail = 1, NULL);
	}
	else
	{
		*sep = '\0';
		if (!is_string_valid(argument))
			return (NULL);
		if (var_exists(env, argument))
			unset_single(argument, env);
		new = env_new_node(argument, sep + 1, FALSE, FALSE);
		if (!new)
			return (*malloc_fail = 1, NULL);
	}
	return (new);
}

int	node_create_fail(t_env **env, t_env *new, char *argument, int malloc_fail)
{
	if (!new && var_exists(env, argument))
		return (0);
	if (!new && malloc_fail)
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		clean_env(env);
		rl_clear_history();
		exit(2);
	}
	return (1);
}

//TODO: malloc fail
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
		new_node = create_exported_var(args[i], env, &malloc_fail);
		if (!new_node)
			return (node_create_fail(env, new_node, args[i], malloc_fail));
		env_add_node(env, new_node);
		i++;
	}
	return (0);
}


