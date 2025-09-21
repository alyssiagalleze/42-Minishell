/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/21 17:12:48 by tfiette          ###   ########.fr       */
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
	t_env	*my_env;

	input = NULL;
	token_list = NULL;
	my_env = init_env_list(env);
	while (1)
	{
		while (!input)
			get_input(&input);
		if (str_cmp(input, "EXIT", FALSE))
		{
			clean_input(&input);
			break ;
		}
		lexer(&token_list, input);
		// if not NULL ?? CHECK when executed;
		parser(&token_list);
		debug_lexer_print_kind(token_list);
		executer(&token_list, 0, 0);
		// if (token_list)
		// {
		// 	test_builtins(token_list, &my_env);
		// }
		clean_token_list(&token_list);
		clean_input(&input);
	}
	clean_env(&my_env); // TODO: pointeur sur env ?
	printf("%d\n", errno);
	// debug_lexer_print(token_list);
	rl_clear_history();
	return (printf("%s", RESET_FONT), 0);
	(void)ac;
	(void)av;
}

//->data struct ??

// verifier synthaxe tjrs ok -> faire debug