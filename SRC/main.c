/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/14 15:00:25 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit(char *input, struct s_data *data) // TODO : built in only
{
	if (str_cmp(input, "exit", FALSE))
		my_exit(0, &data->env, &input, NULL);
}

void	get_input(char **input, struct s_data *data)
{
	while (!*input)
	{
		*input = readline(PROMPT);
		if (is_str_empty_or_null(*input))
			clean_input(input);
		else
			add_history(*input);
	}
	check_exit(*input, data);
}

//TODO : move in correct spot
void	save_last_status(int *status, t_env **env)
{
	t_env	*node;
	
	node = var_exists(env, "?");
	if (!node)
	{
		node = env_new_node("?", "0", FALSE);
		if (node == NULL)
		{
			print_err(PROMPT, PERR_MALLOC, NULL, NULL);
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
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		clean_env(env);
		rl_clear_history();
		exit(2);
	}
}

void	shell_loop(struct s_data *data, int *status)
{
	char *input;

	while (1)
	{
		save_last_status(status, &data->env);
		input = NULL;
		data_reset_pointers(data);
		get_input(&input, data);
		lexer(&data->token_list, input, data);
		if (token_list_size(data->token_list) >= ARG_MAX)
		{
			print_err(PROMPT, PERR_ARG_MAX, NULL, NULL);
			cleaner(NULL, NULL, &data->token_list_head);
			*status = 2;
		}
		else if (parser(&data->token_list))
			*status = token_list_to_exec(data);
		else
			cleaner(NULL, NULL, &data->token_list_head);
		//printf("+++exit status: %i\n", *status);
		// debug_print_env(data->env);
	}
}

int	main(int ac, char **av, char **env)
{
	struct s_data	data;
	int				status;

	data.env = init_env_list(env);
	status = 0;
    // init_signals();
	shell_loop(&data, &status);
	exit_clean(&data);
	return(status);
	(void)ac;
	(void)av;
}

/*
*	signaux
*	readm
*	var= dans l'env
*	passe builtin
*	words type etc in parser
*	rendre claire pipeline exec
*/