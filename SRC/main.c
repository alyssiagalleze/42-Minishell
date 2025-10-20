/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/20 17:51:39 by agalleze         ###   ########.fr       */
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

void	heredoc_input_to_pipe(char *delim, int heredoc_in)
{
	// TODO : unquote delim etc
	char	*input;

	input = NULL;
	while (1)
	{
		input = readline(HDOC_PROMPT);
		if (input)
		{
			if (str_cmp(input, delim, FALSE))
				break;
			write(heredoc_in, input, ft_strlen(input));
			write(heredoc_in, "\n", 1);
			free(input);
		}
	}
	if (input)
		free(input);
}

void	heredocs_display_header(char *delim)
{
	// TODO : unquote delim etc
	write(1, HDOC_HEADER, 11);
	write(1, delim, ft_strlen(delim));
	write(1, "\n", 1);
}

int	heredocs(t_token *token_list)
{
	int		heredoc_pipe[2];
	
	init_heredoc_signals();
	while (token_list)
	{
		heredoc_pipe[0] = -1;
		heredoc_pipe[1] = -1;
		while (token_list && token_list->kind != HDOC)
			token_list = token_list->next;
		if (token_list && token_list->kind == HDOC)
		{
			token_list = token_list->next;
			if (!token_list)
				break ;
			heredocs_display_header(token_list->str);
			if (pipe(heredoc_pipe)/* || dup2(heredoc_pipe[0], STDOUT_FILENO) == -1 */)
			{
				if (heredoc_pipe[0] != -1)
					close(heredoc_pipe[0]);
				if (heredoc_pipe[1] != -1)
					close(heredoc_pipe[1]);
				print_err(PROMPT, "heredoc pipe failed : ", token_list->str, NULL);
				return (ERR_HDOC);
			}
			heredoc_input_to_pipe(token_list->str, heredoc_pipe[1]);
			close(heredoc_pipe[1]);
			token_list->hdoc_fd = heredoc_pipe[0];
			token_list = token_list->next;
		}
	}
	return (ERR_SUCCESS);
}

void	shell_loop(struct s_data *data, int *status)
{
	char *input;

	*status = 0;
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
		input = NULL;
		if (token_list_size(data->token_list) >= ARG_MAX)
		{
			print_err(PROMPT, PERR_ARG_MAX, NULL, NULL);
			cleaner(NULL, NULL, &data->token_list_head);
			*status = 2;
		}
		if (heredocs(data->token_list))
			my_exit(2, &data->env, &input, &data->token_list_head);
		if (parser(&data->token_list))
			*status = token_list_to_exec(data);
		else
			cleaner(&data->env, NULL, &data->token_list_head);
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

	g_signal = 0;
	// loading();
	init_readline_signals();
	data.env = init_env_list(env);
	if (!data.env)
		return (print_err(PROMPT, PERR_MALLOC, NULL, NULL), 1);
	status = 0;
	shell_loop(&data, &status);
	exit_clean(&data);
	return (status);
	(void)ac;
	(void)av;
}

//heredoc :
//prompt user <- quand ?
//
// ouvrir heredoc pipe -> transmettre fd a token_list x
// token_list transmet fd a exec list ?
// clean les fd soit dans token_list soit dans exec list
// -> donc quand fd transmis, le passer a -1;
// recuperer fd a l'exec


// -> var1=theo var2=pierre && echo $var1 $var2

/*
*	heredoc (readline / fd / signals)
*	exit as command (+ args ?)
*	copy env sans local ?
*	clean / valgrind / norminette
*/


// 
