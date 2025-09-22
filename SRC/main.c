/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/22 21:21:40 by tfiette          ###   ########.fr       */
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

void cleaner(t_env **my_env, char **input, t_token **token_list)
{
	clean_env(my_env);
	clean_token_list(token_list);
	clean_input(input);
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_token	*token_list;
	t_token	*token_list_save; //find a way not to use it
	t_env	*my_env;
	int		is_subshell;
	int		status;

	is_subshell = 0;
	while (!is_subshell)
	{
		input = NULL;
		token_list = NULL;
		token_list_save = NULL;
		my_env = init_env_list(env);
		status = 2;
		while (!input)
			get_input(&input);
		if (str_cmp(input, "EXIT", FALSE))
			break ;
		lexer(&token_list, input);
		token_list_save = token_list;
		if (parser(&token_list))
		{
			status = executer(&token_list, 0, 0, &is_subshell);
		}
		cleaner(&my_env, &input, &token_list_save);
		printf("+++exit status: %i\n", status);
	}
	cleaner(&my_env, &input, &token_list_save); // oblige ??
	rl_clear_history();
	if (!is_subshell)
		printf("%s", RESET_FONT);
	printf("return %d\n", status);
	return(status);
	(void)ac;
	(void)av;
}

//->data struct ??
