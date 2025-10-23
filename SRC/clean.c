/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:01:06 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/22 18:43:59 by agalleze         ###   ########.fr       */
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
		if ((*token_list)->hdoc_fd != -1)
		{
			close((*token_list)->hdoc_fd);
			(*token_list)->hdoc_fd = -1;
		}
		free(*token_list);
		*token_list = temp;
	}
	*token_list = NULL;
}

void	clean_env(t_env **env)
{
	printf("Cleaning env\n");
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
	*env = NULL;
}

void	exit_clean(struct s_data *data)
{
	cleaner(&data->env, NULL, &data->token_list_head);
	rl_clear_history();
	//printf("%s", RESET_FONT);
}

void	cleaner(t_env **my_env, char **input, t_token **token_list)
{
	if (my_env && *my_env)
		clean_env(my_env);
	if (token_list && *token_list)
		clean_token_list(token_list);
	if (input && *input)
		clean_input(input);
}
