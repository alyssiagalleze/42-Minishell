/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_bis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 17:19:46 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/23 17:58:37 by agalleze         ###   ########.fr       */
/*                                                                            */
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

int	invalid_first_char(char c)
{
	return (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c < 'Z')
		&& c != '_');
}

int	invalid_identifier(char *str)
{
	return (!(*str >= 'a' && *str <= 'z') && !(*str >= 'A' && *str < 'Z')
		&& *str != '_' && !(*str >= '0' && *str < '9'));
}

int	is_string_valid_var(char *str)
{
	int	is_valid;

	is_valid = TRUE;
	if (!str || (invalid_first_char(*str)))
		is_valid = FALSE;
	while (is_valid && str && *str)
	{
		if (invalid_identifier(str))
		{
			is_valid = FALSE;
			break ;
		}	
		str++;
	}
	if (!is_valid)
	{
		print_err(PROMPT, ": export: \'", str, "\' : not a valid identifier\n");
		return (FALSE);
	}
	return (TRUE);
}
