/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:25:01 by agalleze          #+#    #+#             */
/*   Updated: 2025/09/16 13:34:13 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_env **env, char *name)
{
	t_env	*prev;
	t_env	*current;

	prev = NULL;
	current = *env;
	while (current)
	{
		if (str_cmp(current->var_name, name, FALSE))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->var_name);
			free(current->var_value);
			free(current);
			break;
		}
		prev = current;
		current = current->next;
	}
}
