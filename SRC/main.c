/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/30 18:58:34 by agalleze         ###   ########.fr       */
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
/* 
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
} */


void cleaner(t_env **my_env, char **input, t_token **token_list)
{
	if (my_env)
		clean_env(my_env);
	if (input)
		clean_token_list(token_list);
	if (token_list)
		clean_input(input);
}

// int g_exit_status = 0;


int	main(int ac, char **av, char **env)
{
	char	*input;
	t_token	*token_list;
	t_token	*token_list_save; //find a way not to use it
	t_env	*my_env;
	int		status;

	my_env = init_env_list(env);
	status = 2;
    init_signals();
	while (1)
	{
		input = NULL;
		token_list = NULL;
		token_list_save = NULL;
		while (!input)
			get_input(&input, status);
		if (str_cmp(input, "exit", FALSE))
			my_exit(status, &my_env, &input, &token_list_save);
		lexer(&token_list, input);
		token_list_save = token_list;
		if (parser(&token_list))
		{
			status = lister(&token_list, &my_env);
		}
		cleaner(NULL, &input, &token_list_save);
		printf("+++exit status: %i\n", status);
	}
	cleaner(&my_env, &input, &token_list_save); // oblige ??
	rl_clear_history();
	printf("return %d\n", status);
	return(status);
	(void)ac;
	(void)av;
}

//->data struct ??
