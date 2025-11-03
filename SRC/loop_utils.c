/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 10:55:51 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 11:03:05 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_std_errors(void)
{
	if (!isatty(STDIN_FILENO))
	{
		print_err(PROMPT, E_STDIN, E_REDIR, NULL);
		exit (2);
	}
	if (!isatty(STDOUT_FILENO))
	{
		print_err(PROMPT, E_STDOUT, E_REDIR, NULL);
		exit (2);
	}
}

static void	check_exit(char **input, struct s_data *data, int status)
{
	if (!*input)
		my_exit(status, &data->env, NULL, NULL);
	if (str_cmp(*input, "exit", FALSE))
		my_exit(status, &data->env, input, NULL);
}

void	save_last_status(int *status, t_env **env)
{
	t_env	*node;

	node = var_exists(env, "?");
	if (!node)
	{
		node = env_new_node("?", "0", FALSE, TRUE);
		if (node == NULL)
		{
			print_err(PROMPT, E_MALLOC, NULL, NULL);
			clean_env(env);
			rl_clear_history();
			exit(2);
		}
		env_add_node(env, node);
		return ;
	}
	free(node->var_value);
	node->var_value = ft_itoa(*status);
	if (node->var_value == NULL)
	{
		print_err(PROMPT, E_MALLOC, NULL, NULL);
		clean_env(env);
		rl_clear_history();
		exit(2);
	}
}

void	get_input(
	char **input, struct s_data *data, int *status, int *cmd_count)
{
	*input = NULL;
	while (!*input)
	{
		*input = readline(PROMPT);
		if (g_signal)
		{
			*status = g_signal + 128;
			save_last_status(status, &data->env);
			g_signal = 0;
		}
		check_exit(input, data, *status);
		*cmd_count += 1;
		if (is_str_empty_or_null(*input))
			clean_input(input);
		else
			add_history(*input);
	}
}

int	is_longer_than_arg_max(struct s_data *data, int *status)
{
	if (token_list_size(data->token_list) >= ARG_MAX)
	{
		print_err(PROMPT, E_ARG_MAX, NULL, NULL);
		cleaner(NULL, NULL, &data->token_list_head);
		*status = 2;
		return (TRUE);
	}
	return (FALSE);
}
