/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/10 11:07:45 by tfiette          ###   ########.fr       */
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

void	data_reset_pointers(struct s_data *data)
{
	data->token_list = NULL;
	data->token_list_save = NULL;
}

void	data_save_token_list(struct s_data *data)
{
	data->token_list_save = data->token_list;
}

void	shell_loop(struct s_data *data, int *status)
{
	int	prev_fd = -1;
	char *input;

	while (1)
	{
		input = NULL;	
		data_reset_pointers(data);
		while (!input)
			get_input(&input);
		if (str_cmp(input, "exit", FALSE))
			my_exit(0, &data->env, &input, &data->token_list_save); // TODO : normalement pas besoin de free sauf input
		lexer(&data->token_list, input);
		data_save_token_list(data);
		free(input);
		if (token_list_size(data->token_list) >= ARG_MAX)
		{
			print_err(PROMPT, PERR_ARG_MAX, NULL, NULL);
			cleaner(NULL, NULL, &data->token_list_save);
		}
		else if (parser(&data->token_list))
			*status = lister(data, &prev_fd);
		else
			cleaner(NULL, NULL, &data->token_list_save);
		printf("+++exit status: %i\n", *status);
	}
}

void	exit_clean(struct s_data *data)
{
	cleaner(&data->env, NULL, &data->token_list_save);
	rl_clear_history();
}

// est ce que j'envoie le pointeur ou le pointeur sur pointeur ?
int	main(int ac, char **av, char **env)
{
	struct s_data	data;
	int				status;

	data.env = init_env_list(env);
	status = 2;
    // init_signals();
	shell_loop(&data, &status);
	exit_clean(&data);
	printf("%s", RESET_FONT);
	return(status);
	(void)ac;
	(void)av;
}
