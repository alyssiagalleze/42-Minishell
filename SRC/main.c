/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/24 19:20:20 by tfiette          ###   ########.fr       */
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

void	get_input(
	char **input, struct s_data *data, int *status, int *cmd_count)
{
	*input = NULL;
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

void	prev_command_check_signal(int	*status)
{
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
}

int	heredoc_check_signal(struct s_data *data, int *status)
{
	if (g_signal == SIGINT)
	{
		cleaner(NULL, NULL, &data->token_list_head);
		*status = SIGINT + 128;
		g_signal = 0;
		return (TRUE);
	}
	return (FALSE);
}

int	is_longer_than_arg_max(struct s_data *data, int *status)
{
	if (token_list_size(data->token_list) >= ARG_MAX)
	{
		print_err(PROMPT, PERR_ARG_MAX, NULL, NULL);
		cleaner(NULL, NULL, &data->token_list_head);
		*status = 2;
		return (TRUE);
	}
	return (FALSE);
}

void	shell_loop(struct s_data *data, int *status)
{
	char	*input;
	int		cmd_count;

	*status = 0;
	cmd_count = 0;
	while (1)
	{
		init_readline_signals();
		prev_command_check_signal(status);
		g_signal = 0;
		save_last_status(status, &data->env);
		data_reset_pointers(data);
		get_input(&input, data, status, &cmd_count);
		lexer(&data->token_list, input, data);
		if (is_longer_than_arg_max(data, status))
			continue;
		else if (heredocs(data->token_list, cmd_count, data->env))
			my_exit(2, &data->env, &input, &data->token_list_head);
		if (heredoc_check_signal(data, status))
			continue ;
		if (parser(&data->token_list, &data->token_list_head, status))
			*status = token_list_to_exec(data);
	}
}

void	check_std_errors()
{
	if (!isatty(STDIN_FILENO))
	{
		print_err(PROMPT, PERR_STDIN, PERR_REDIR, NULL);
		exit (2);
	}
	if (!isatty(STDOUT_FILENO))
	{
		print_err(PROMPT, PERR_STDOUT, PERR_REDIR, NULL);
		exit (2);
	}
}

int g_signal = 0;

int	main(int ac, char **av, char **env)
{
	struct s_data	data;
	int				status;

	(void)ac;
	(void)av;
	check_std_errors();
	g_signal = 0;
	if (init_env_list(env, &data.env))
		return (print_err(PROMPT, PERR_MALLOC, NULL, NULL), 2);
	status = 0;
	shell_loop(&data, &status);
	exit_clean(&data);
	return (status);
}
