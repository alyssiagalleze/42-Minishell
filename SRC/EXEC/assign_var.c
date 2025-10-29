/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:47:54 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/28 17:48:27 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO: clean malloc
//TODO: exit close etc
pid_t	exec_assign_var(t_exec *exec_list, t_env **env)
{
	t_env	*node;
	int		i;
	char	*var_value;
	char	*var_name;

	var_name = exec_list->command->argv[0];
	i = is_char_in_string('=', var_name, FALSE, TRUE);
	var_name[i] = '\0';
	if (!is_string_valid_var(var_name))
	{
		return (-1);
	}
	var_value = ft_strdup(var_name + i + 1);
	if (!var_value)
		return (print_err(PROMPT, PERR_ASSIGN, var_name, NULL), -2);
	node = var_exists(env, var_name);
	if (!node)
	{
		node = env_new_node(var_name, var_value, FALSE, TRUE);
		free(var_value);
		if (!node)
			return (print_err(PROMPT, PERR_ASSIGN, var_name, NULL), -2);
		env_add_node(env, node);
	}
	else
	{
		if (node->var_value)
			free(node->var_value);
		node->var_value = var_value;
		node->is_exported = FALSE;
		node->is_local = TRUE;
	}
	return (0);
}

int	is_var(t_exec *exec_list)
{
	return (exec_list->command->is_var);
}
