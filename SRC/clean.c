/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:01:06 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/23 16:52:25 by tfiette          ###   ########.fr       */
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
			ft_close(&(*token_list)->hdoc_fd);
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
	write(1, RESET_FONT, 1);
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

void	clean_data_close_fds(struct s_exec_data *exec_data, t_exec *exec_list)
{
	if (*(&exec_data->env))
		clean_env(&exec_data->env);
	if (*(&exec_data->token_list))
	{
		printf("Cleaning token list in exec_data\n");
		clean_token_list(&exec_data->token_list);
	}
	if (exec_data->saved_stds[0] != -1 || exec_data->saved_stds[1] != -1)
		restore_std_fds(exec_data->saved_stds);
	if (exec_data->prev_fd != -1)
		close(exec_data->prev_fd);
	close_fds(exec_data->pipefds, exec_data->saved_stds);
	if (*(&exec_list))
		clean_exec_list(&exec_list);
}
