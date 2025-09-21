/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:01:06 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/19 17:25:51 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_input(char **input)
{
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
}

void	clean_token_list(t_token	**lexer)
{
	t_token	*temp_node;

	while ((*lexer))
	{
		temp_node = (*lexer)->next;
		if ((*lexer)->str)
			free((*lexer)->str);
		free((*lexer));
		(*lexer) = temp_node;
	}
	*lexer = NULL;
}

void	clean_env(t_env **env)
{
	t_env	*tmp;

	while (*env)
	{
		tmp = (*env)->next;
		if ((*env)->var_name)
			free((*env)->var_name);
		if ((*env)->var_value)
			free((*env)->var_value);
		free(*env);
		*env = tmp;
	}
	env = NULL;
}
