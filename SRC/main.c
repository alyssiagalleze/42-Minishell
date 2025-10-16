/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/16 13:49:20 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit(char **input, struct s_data *data, int status)
{
	if (!*input)
		my_exit(status, &data->env, NULL, NULL);
	if (str_cmp(*input, "exit", FALSE))
		my_exit(status, &data->env, input, NULL);
}

//TODO : check quel std
void	get_input(char **input, struct s_data *data, int status)
{
	while (!*input)
	{
		*input = readline(PROMPT);
		check_exit(input, data, status);
		if (is_str_empty_or_null(*input))
			clean_input(input);
		else
			add_history(*input);
	}
}

//TODO : move in correct spot
void	save_last_status(int *status, t_env **env)
{
	t_env	*node;
	
	node = var_exists(env, "?");
	if (!node)
	{
		node = env_new_node("?", "0", FALSE, TRUE);
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
		if (g_signal == SIGINT || *status == SIGINT + 128)
		{
			write(STDIN_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		if (g_signal == SIGQUIT || *status == SIGQUIT + 128)
		{
			write(STDIN_FILENO, "Quit (core dumped)\n", 20);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		g_signal = 0;
		save_last_status(status, &data->env);
		input = NULL;
		data_reset_pointers(data);
		get_input(&input, data, *status);
		if (g_signal)
		{
			*status = g_signal + 128;
			save_last_status(status, &data->env);
			g_signal = 0;
		}
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
	}
}

// void	loading()
// {
// 	int	j;
// 	int	k;
// 	int bloc = '=';
	
// 	write(1, "\1\e[1;38;5;82m\2\1\e[0;38;5;82m\2", 30);
// 	write(1, "==============================================================================\n", 80);
// 	write(1, "                                 loading... \n", 46);
// 	j = 0;
// 	while (j < 80)
// 	{
		
// 	}
// 	write(1, "\n", 1);
// }

int g_signal = 0;

int	main(int ac, char **av, char **env)
{
	struct s_data	data;
	int				status;

	// loading();
	init_readline_signals();
	data.env = init_env_list(env);
	status = 0;
	shell_loop(&data, &status);
	exit_clean(&data);
	return(status);
	(void)ac;
	(void)av;
}

/*
*	heredoc (readline / fd / signals)
*	
*	exit as command (+ args ?)
*	copy env sans local ?
*	clean / valgrind / norminette
*/