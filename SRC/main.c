/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/17 17:24:39 by agalleze         ###   ########.fr       */
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

// ls && echo -n "input for test" || echo "&& test et test && (echo a || echo b)"

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_lexer	*lexer;
	t_lexer	*lexer_node;
	t_env	*my_env;
	char	**input_tab;
	
	input = NULL;
	lexer = NULL;
	lexer_node = NULL;
	my_env = init_env_list(env);
	while (1)
	{
		// clean_lexer(&lexer);
		while (!input)
			get_input(&input);
		if (str_cmp(input, "EXIT", FALSE))
		{
			clean_input(&input);
			break;
		}
		input_tab = ft_split(input, ' ');
		if (!input_tab)
		break;
		if (str_cmp(input_tab[0], "cd", FALSE))
		{
			printf("-----%s\n", input_tab[1]);
			cd(input_tab[1], &my_env);
		}
		if (str_cmp(input_tab[0], "env", FALSE))
		print_env(&my_env);	
		if (str_cmp(input_tab[0], "pwd", FALSE))
		pwd();	
		if (str_cmp(input_tab[0], "echo", FALSE))
		echo(input_tab + 1);
		if (str_cmp(input_tab[0], "unset", FALSE))
			unset(&my_env, input_tab[1]);	
		if (str_cmp(input_tab[0], "export", FALSE))
			errno = export(input_tab + 1, &my_env);	
		//faire le lexer sur l'input et separer word / operators
		lexer_parse_input(&lexer, input);
		//STOP WORKING // STOP WORKING // STOP WORKING
		clean_input(&input);
		clean_lexer(lexer);
	}
	printf("%d\n", errno);
	// debug_lexer_print(lexer);
	// clean_env(my_env);
	rl_clear_history();
	(void)ac;
	(void)av;
	return (printf("%s", RESET_FONT), 0);
}

//->lexer
//->data struct ??

