/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/23 11:59:04 by tfiette          ###   ########.fr       */
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

void	get_input(char **input, struct s_data *data, int *status, int *cmd_count)
{
	while (!*input)
	{
		*input = readline(PROMPT);
		if (g_signal)
		{
			*status = g_signal + 128;
			save_last_status(status, &data->env);
			g_signal = 0;
		}
		check_exit(input, data, *status);
		*cmd_count += 1;
		if (is_str_empty_or_null(*input))
			clean_input(input);
		else
			add_history(*input);
	}
}

// -> str_append qui rajoute une chaine de caractere a une autre (doit etre malloc)

// -> avance dans input
// -> recupere la len de soit 	1) une chaine sans expand
//									-> recupere copie de chaine
// 								2) une chaine a expand
//									-> recupere copie de var equivalente
//	append la copie a new_string

void	shell_loop(struct s_data *data, int *status)
{
	char 	*input;
	int		cmd_count;

	*status = 0;
	cmd_count = 0;
	while (1)
	{
		init_readline_signals();
		if (g_signal == SIGINT || *status == SIGINT + 128)
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		if (g_signal == SIGQUIT || *status == SIGQUIT + 128)
		{
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		g_signal = 0;
		save_last_status(status, &data->env);
		input = NULL;
		data_reset_pointers(data);
		get_input(&input, data, status, &cmd_count);
		lexer(&data->token_list, input, data);
		input = NULL;
		if (token_list_size(data->token_list) >= ARG_MAX)
		{
			print_err(PROMPT, PERR_ARG_MAX, NULL, NULL);
			cleaner(NULL, NULL, &data->token_list_head);
			*status = 2;
			continue ;
		}
		else if (heredocs(data->token_list, cmd_count, data->env))
			my_exit(2, &data->env, &input, &data->token_list_head); //TODO : le retour se fait dans fonction heredoc ?
		if (g_signal == SIGINT) //gerer ca directement dans heredoc ?
		{
			cleaner(NULL, NULL, &data->token_list_head);
			*status = SIGINT + 128;
			g_signal = 0;
			continue ;
		}
		if (parser(&data->token_list))
			*status = token_list_to_exec(data);
		else
		{
			cleaner(NULL, NULL, &data->token_list_head);
			*status = 2;
		}
	}
}

int g_signal = 0;

int	main(int ac, char **av, char **env)
{
	struct s_data	data;
	int				status;

	(void)ac;
	(void)av;
	if (!isatty(STDIN_FILENO))
	{
		print_err(PROMPT, "Incident with the STDIN\n", "You might have redirected it, which is not supported.\n", NULL);
		exit (2);
	}
	if (!isatty(STDOUT_FILENO))
	{
		print_err(PROMPT, "Incident with the STDOUT\n", "You might have redirected it, which is not supported.\n", NULL);
		exit (2);
	}
	g_signal = 0;
	if (init_env_list(env, &data.env))
		return (print_err(PROMPT, PERR_MALLOC, NULL, NULL), 2);
	status = 0;
	shell_loop(&data, &status);
	exit_clean(&data);
	return (status);
}

// TO FIX !!

// mkdir -> rm ../. -> pwd ou cd ..

