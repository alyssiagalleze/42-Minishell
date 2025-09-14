/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/14 19:51:09 by tfiette          ###   ########.fr       */
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

int	main()
{
	char	*input;
	t_lexer	*lexer;
	t_lexer	*lexer_node;
	
	input = NULL;
	lexer = NULL;
	lexer_node = NULL;
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
		//faire le lexer sur l'input et separer word / operators
		lexer_parse_input(&lexer, input);
		//STOP WORKING // STOP WORKING // STOP WORKING
		clean_input(&input);
		clean_lexer(lexer);
	}
	// debug_lexer_print(lexer);
	rl_clear_history();
	return (printf("%s", RESET_FONT), 0);
}

//->lexer
//->data struct ??

