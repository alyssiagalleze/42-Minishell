/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/11 15:39:17 by tfiette          ###   ########.fr       */
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

void	shell_loop(struct s_data *data, int *status)
{
	char *input;

	while (1)
	{
		input = NULL;
		data_reset_pointers(data);
		get_input(&input, data);
		lexer(&data->token_list, input, data);
		if (token_list_size(data->token_list) >= ARG_MAX)
		{
			print_err(PROMPT, PERR_ARG_MAX, NULL, NULL);
			cleaner(NULL, NULL, &data->token_list_head);
		}
		else if (parser(&data->token_list))
			*status = token_list_to_exec(data);
		else
			cleaner(NULL, NULL, &data->token_list_head);
		printf("+++exit status: %i\n", *status);
	}
}

int	main(int ac, char **av, char **env)
{
	struct s_data	data;
	int				status;

	data.env = init_env_list(env);
	status = 2;
    // init_signals();
	shell_loop(&data, &status);
	exit_clean(&data);
	return(status);
	(void)ac;
	(void)av;
}

/*
*	norminette first passe
*	passe builtin
*	words type etc in parser
*	rendre claire pipeline exec
*
*/