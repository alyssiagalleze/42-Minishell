/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/01 16:43:41 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_input(char **input)
{
	*input = readline(PROMPT);
	if (is_str_empty_or_null(*input))
		clean_input(input);
	else
		add_history(*input);
}

void	test_builtins(t_token *lexer, t_env **my_env)
{
	t_token	*curr_node;

	curr_node = lexer;
	while (curr_node)
	{
		if (str_cmp(curr_node->str, "cd", FALSE))
		{
			printf("-----%s\n", curr_node->next->str);
			cd(curr_node->next->str, my_env);
		}
		if (str_cmp(curr_node->str, "env", FALSE))
			print_env(my_env);
		if (str_cmp(curr_node->str, "pwd", FALSE))
			pwd();
		if (str_cmp(curr_node->str, "echo", FALSE))
			echo(&(curr_node->next->str));
		if (str_cmp(curr_node->str, "unset", FALSE))
			unset(my_env, curr_node->next->str);
		if (str_cmp(curr_node->str, "export", FALSE))
			errno = export(&(curr_node->next->str), my_env);
		curr_node = curr_node->next;
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_token	*token_list;
	t_token	*token_list_save; //find a way not to use it
	t_env	*my_env;
	int		status;

	my_env = init_env_list(env);
	while (1)
	{
		input = NULL;
		token_list = NULL;
		token_list_save = NULL;
		status = 2;
		while (!input)
			get_input(&input);
		if (str_cmp(input, "EXIT", FALSE))
			break ;
		lexer(&token_list, input);
		token_list_save = token_list;
		if (parser(&token_list))
		{
			status = lister(&token_list, &my_env, &input, &token_list_save);
		}
		printf("+++exit status: %i\n", status);
	}
	cleaner(&my_env, &input, &token_list_save); // oblige ??
	rl_clear_history();
	printf("%s", RESET_FONT);
	return(status);
	(void)ac;
	(void)av;
}

//->data struct ??
