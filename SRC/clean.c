/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:01:06 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/30 12:10:25 by agalleze         ###   ########.fr       */
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

void	clean_token_list(t_token	**token_list)
{
	t_token	*token;

	while (*token_list)
	{
		token = (*token_list)->next;
		if ((*token_list)->str)
			free((*token_list)->str);
		if (*token_list)
			free(*token_list);
		*token_list = token;
	}
	*token_list = NULL;
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

void	exec_cleaner(char **path_tab, char *path)
{
	int	i;

	i = 0;
	if (path_tab)
	{
		while (path_tab[i])
			free(path_tab[i++]);
		free(path_tab);
	}
	if (path)
		free(path);
}
