/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/09 12:17:27 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_input(char **input, int status)
{
	*input = readline(PROMPT);
	// if (*input == NULL)
	// 	exit(0); // ici appliquer la fonction exit qui utilisera le bon exit status 
	if (is_str_empty_or_null(*input, status))
		clean_input(input);
	else
		add_history(*input);
}

int	token_list_size(t_token *token_list)
{
	int	i;

	i = 0;
	while (token_list)
	{
		i ++;
		if (i == ARG_MAX)
			return (ARG_MAX);
		token_list = token_list->next;
	}
	return (i);
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_token	*token_list;
	t_token	*token_list_save; //find a way not to use it
	t_env	*my_env;
	int		status;
	int prev_fd = -1;

	my_env = init_env_list(env);
	status = 2;
    // init_signals();
	while (1)
	{
		input = NULL;
		token_list = NULL;
		token_list_save = NULL;
		while (!input)
			get_input(&input, status);
		if (str_cmp(input, "exit", FALSE))
			my_exit(0, &my_env, &input, &token_list_save);
		lexer(&token_list, input);
		token_list_save = token_list;
		if (token_list_size(token_list) >= ARG_MAX)
		{
			print_err(PROMPT, PERR_ARG_MAX, NULL, NULL);
			cleaner(NULL, &input, &token_list_save);
		}
		else if (parser(&token_list))
			status = lister(&token_list, &my_env, &input, &token_list_save);
		else
			cleaner(NULL, &input, &token_list_save);
		printf("+++exit status: %i\n", status);
	}
	cleaner(&my_env, &input, &token_list_save); // oblige ??
	rl_clear_history();
	printf("%s", RESET_FONT);
	return(status);
	(void)ac;
	(void)av;
}
