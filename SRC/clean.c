/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:01:06 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/07 13:42:00 by tfiette          ###   ########.fr       */
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
	t_token	*temp;

	while (*token_list)
	{
		temp = (*token_list)->next;
		if ((*token_list)->str)
		{
			free((*token_list)->str);
			(*token_list)->str = NULL;
		}
		free(*token_list);
		*token_list = temp;
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

void	clean_exec_list(t_exec **exec_list)
{
	t_exec	*temp;
	int		i;
	
	while (*exec_list)
	{
		temp = (*exec_list)->next;
		i = 0;
		if ((*exec_list)->is_command)
		{
			while ((*exec_list)->command->argv[i])
			{
				free((*exec_list)->command->argv[i]);
				(*exec_list)->command->argv[i] = NULL;
				i ++;
			}
			free((*exec_list)->command);
		}
		else if ((*exec_list)->is_subshell)
		{
			if ((*exec_list)->subshell->token_sublist)
				clean_token_list(&((*exec_list)->subshell->token_sublist));
			free((*exec_list)->subshell);
		}
		free(*exec_list);
		*exec_list = temp;
	}
	*exec_list = NULL;
}

void cleaner(t_env **my_env, char **input, t_token **token_list)
{
	if (my_env && *my_env)
		clean_env(my_env);
	if (token_list && *token_list)
		clean_token_list(token_list);
	if (input && *input)
		clean_input(input);
}
