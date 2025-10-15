/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:25:01 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/15 19:18:30 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

int	unset_single(char *arg, t_env **env)
{
	t_env	*prev;
	t_env	*current;
	
	prev = NULL;
	current = *env;
	while (current)
	{
		if (str_cmp(current->var_name, arg, FALSE))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->var_name);
			free(current->var_value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}

int	unset(char **args, t_env **env)
{
	t_env	*prev;
	t_env	*current;
	int		i;
	
	i = 0;
	prev = NULL;
	current = *env;
	while (args[i])
	{
		current = *env;
		while (current)
		{
			if (str_cmp(current->var_name, args[i], FALSE))
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
		i++;
		
	}
	return (0);
}
