/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:01:06 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/24 17:59:38 by tfiette          ###   ########.fr       */
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

void	clean_token_list(t_token **token_list, int close_sub_fds)
{
	t_token	*temp;
	int		subshell_lvl;

	subshell_lvl = 0;
	while (*token_list)
	{
		temp = (*token_list)->next;
		if ((*token_list)->kind == BRACKET_O)
			subshell_lvl++;
		if ((*token_list)->kind == BRACKET_C)
			subshell_lvl--;
		if ((*token_list)->str)
		{
			free((*token_list)->str);
			(*token_list)->str = NULL;
		}
		if (close_sub_fds || (!subshell_lvl && (*token_list)->hdoc_fd != -1))
			ft_close(&(*token_list)->hdoc_fd);
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
	*env = NULL;
}

void	exit_clean(struct s_data *data)
{
	cleaner(&data->env, NULL, &data->token_list_head);
	rl_clear_history();
	write(1, RESET_FONT, 1);
}

void	cleaner(t_env **my_env, char **input, t_token **token_list)
{
	if (my_env && *my_env)
		clean_env(my_env);
	if (token_list && *token_list)
		clean_token_list(token_list, TRUE);
	if (input && *input)
		clean_input(input);
}
