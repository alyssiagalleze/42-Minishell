/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   my_env_list.c									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agalleze <agalleze@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/09/16 10:50:49 by agalleze		  #+#	#+#			 */
/*   Updated: 2025/09/16 12:24:03 by agalleze		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

t_env   *env_new_node(const char *var_name, const char *var_value, int exported)
{
	t_env   *node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->var_name = strdup(var_name);
	if (var_value)
		node->var_value = strdup(var_value);
	else
		node->var_value = NULL;
	node->is_exported = exported;
	node->next = NULL;
	return (node);
}

void	env_add_node(t_env **top, t_env *node)
{
	t_env   *tmp;

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

t_env   *init_env_list(char **env)
{
	t_env   *top;
	char	*var_name;
	char	*var_value;
	char	*sep;
	int	 i;

	top = NULL;
	i = 0;
	while (env[i])
	{
		sep = strchr(env[i], '=');
		if (sep)
		{
			*sep = '\0';
			var_name = env[i];
			var_value = sep + 1;
			env_add_node(&top, env_new_node(var_name, var_value, FALSE));
			*sep = '=';
		}
		i++;
	}
	return (top);
}

int	update_variable(t_env **env, char *var, char *value)
{
	unset(env, var);
	env_add_node(env, env_new_node(var, value, FALSE));
	return (0);
}

char	*get_var_value(t_env **env, char *var_name)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (str_cmp(current->var_name, var_name, FALSE))
			return (strdup(current->var_value));
		current = current->next;
	}
	return (NULL);
}


