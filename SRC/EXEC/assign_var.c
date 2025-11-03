/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:47:54 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 12:10:34 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	exec_assign_new_var(
	t_env **env, t_env *node, char *var_name, char *var_value)
{
	node = env_new_node(var_name, var_value, FALSE, TRUE);
	free(var_value);
	if (!node)
	{
		print_err(PROMPT, E_ASSIGN, var_name, NULL);
		return (-2);
	}
	env_add_node(env, node);
	return (0);
}

static pid_t	exec_assign_existing_var(t_env *node, char *var_value)
{
	if (node->var_value)
		free(node->var_value);
	node->var_value = var_value;
	node->is_exported = FALSE;
	node->is_local = TRUE;
	return (0);
}

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
		return (-1);
	var_value = ft_strdup(var_name + i + 1);
	if (!var_value)
		return (print_err(PROMPT, E_ASSIGN, var_name, NULL), -2);
	node = var_exists(env, var_name);
	if (!node)
		return (exec_assign_new_var(env, node, var_name, var_value));
	return (exec_assign_existing_var(node, var_value));
}

int	is_var(t_exec *exec_list)
{
	return (exec_list->command->is_var);
}
