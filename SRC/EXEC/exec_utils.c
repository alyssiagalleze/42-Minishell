/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:41:11 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/22 15:32:28 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_env_variable(t_env *node)
{
	char	*var;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(node->var_name) + ft_strlen(node->var_value) + 2;
	var = malloc(len);
	if (!var)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len && node->var_name[j])
		var[i++] = node->var_name[j++];
	var[i] = '=';
	i++;
	j = 0;
	while (i < len && node->var_value[j])
		var[i++] = node->var_value[j++];
	var[i] = '\0';
	return (var);
}

int	transfer_env(t_env **env, char ***new_env)
{
	t_env	*current;
	char	**my_env;
	int		lst_len;
	int		i;

	lst_len = env_size(*env);
	i = 0;
	my_env = malloc((lst_len + 1) * (sizeof(char *)));
	if (!my_env)
		return (ERR_SUCCESS);
	current = *env;
	while (current)
	{
		if (!current->is_local)
		{
			my_env[i] = build_env_variable(current);
			if (!my_env[i])
				return (ERR_MALLOC);
			i ++;
		}	
		current = current->next;
	}
	my_env[i] = NULL;
	*new_env = my_env;
	return (ERR_SUCCESS);
}
