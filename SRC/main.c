/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/20 18:37:33 by tfiette          ###   ########.fr       */
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

void	get_input(char **input, struct s_data *data, int status, int *cmd_count)
{
	while (!*input)
	{
		*input = readline(PROMPT);
		check_exit(input, data, status);
		*cmd_count += 1;
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

// -> str_append qui rajoute une chaine de caractere a une autre (doit etre malloc)

// -> avance dans input
// -> recupere la len de soit 	1) une chaine sans expand
//									-> recupere copie de chaine
// 								2) une chaine a expand
//									-> recupere copie de var equivalente
//	append la copie a new_string

//TODO : 5 args
int	heredoc_expand_input(char **input, t_env *env)
{
	char	*input_save;
	char	*new_input;
	char	*substr;
	t_env	*var;
	int		len;

	input_save = *input;
	new_input = NULL;
	while (**input)
	{
		len = 0;
		if (**input != '$')
		{
			while ((*input)[len] && (*input)[len] != '$')
				len++;
			// printf("str len = %d\n", len);
			substr = extract_string(*input, len);
			if (substr == NULL)
			{
				// TODO : malloc error
			}
			new_input = str_append(new_input, substr);
			if (new_input == NULL)
			{
				// TODO : malloc error
			}
			*input += len;
		}
		len = 0;
		if (**input == '$')
		{
			len++;
			if (!is_char_in_string((*input)[len], "0123456789", FALSE, FALSE))
			{
				while (is_expandable_char((*input)[len]))
				{
					// printf("char is expendable %c\n", (*input)[len]);
					len++;
				}
				// printf("char is NOT expendable %c\n", (*input)[len]);
			}
			// printf("var len = %d\n", len);
			if (len == 1)
				substr = ft_strdup("$");
			else
			{
				substr = extract_string(*input + 1, len - 1);
				// printf("looking for substr from string %s\n", *input);
				if (substr == NULL)
				{
					// TODO : malloc error
				}
				var = var_exists(&env, substr);
				free(substr);
				if (var == NULL)
				{
					substr = NULL;
					*input += len;
					continue ;
				}
				substr = ft_strdup(var->var_value);
			}	
			if (substr == NULL)
			{
				// TODO : malloc error
			}
			new_input = str_append(new_input, substr);
			if (new_input == NULL)
			{
				// TODO : malloc error
			}
			*input += len;
		}
	}
	(void)env;
	free(input_save);
	*input = new_input;
	// printf("Expanded %s <-\n", new_input);
	return (ERR_SUCCESS);
}

int	heredoc_input_to_pipe(char *delim, int	heredoc_in, int *cmd_count, int no_expand, t_env *env)
{
	char	*input;
	int		hdoc_len;
	char	*line_count_str;
	int		err;

	input = NULL;
	hdoc_len = 0;
	while (1)
	{
		input = readline(HDOC_PROMPT);
		if (g_signal == SIGINT)
			return (ERR_SUCCESS);
		if (input && str_cmp(input, delim, FALSE))
			break ;
		if (input)
		{
			if (!is_str_empty_or_null(input) && !no_expand)
			{
				err = heredoc_expand_input(&input, env);
				if (err)
					return (err);
			}
			hdoc_len ++;
			write(heredoc_in, input, ft_strlen(input));
			write(heredoc_in, "\n", 1);
			free(input);
			*cmd_count += 1;
		}
		else if (!input)
		{
			*cmd_count += 1;
			line_count_str = ft_itoa(*cmd_count);
			if (line_count_str)
			{
				print_err("XxxM3g4sh311xxX: warning: here-document at line ", line_count_str, " delimited by end-of-file\n", NULL);
				free(line_count_str);
			}			
			else
				print_err("XxxM3g4sh311xxX: warning: here-document delimited by end-of-file (wanted `", delim, "/')\n", NULL);
			break ;
		}
	}
	if (input)
		free(input);
	return (ERR_SUCCESS);
}

void	heredocs_display_header(char *delim)
{
	// TODO : unquote delim etc
	write(1, HDOC_HEADER, 11);
	write(1, delim, ft_strlen(delim));
	write(1, "\n", 1);
}

int	heredoc_expand_delim(char *delim, char **new_delim, int *is_quoted)
{
	int	len;
	int	i;
	int	j;
	char	c;

	i = 0;
	j = 0;
	len = ft_strlen(delim);
	*new_delim = malloc(sizeof(*delim) * (len + 1));
	if (!*new_delim)
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	*is_quoted = FALSE;
	while (i < len)
	{
		if (delim[i] == '\'' || delim[i] == '"')
		{
			c = delim[i];
			*is_quoted = TRUE;
			i++;
			while (i < len && delim[i] != c)
			{
				(*new_delim)[j] = delim[i];
				j++;
				i++;
			}
			if (i >= len)
			{
				print_err(PROMPT, PERR_STX_HDOC, NULL, NULL);
				free(*new_delim);
				return (ERR_AMBIG);
			}
			i++;
			c = '\0';
		}
		while (i < len && delim[i] != '\'' && delim[i] != '"')
		{
			(*new_delim)[j] = delim[i];
			j++;
			i++;
		}
	}
	(*new_delim)[j] = '\0';
	return (ERR_SUCCESS);
}

int	heredocs(t_token *token_list, int cmd_count, t_env *env)
{
	int		heredoc_pipe[2];
	char	*exp_delim;
	int		is_quoted;
	int		err;
	
	exp_delim = NULL;
	while (token_list)
	{
		heredoc_pipe[0] = -1;
		heredoc_pipe[1] = -1;
		init_heredoc_signals();
		while (token_list && token_list->kind != HDOC)
			token_list = token_list->next;
		if (token_list && token_list->kind == HDOC)
		{
			token_list = token_list->next;
			if (!token_list)
			break ;
			err = heredoc_expand_delim(token_list->str, &exp_delim, &is_quoted);
			if (err)
			return (err);
			heredocs_display_header(exp_delim);
			if (pipe(heredoc_pipe))
			{
				if (heredoc_pipe[0] != -1)
					close(heredoc_pipe[0]);
				if (heredoc_pipe[1] != -1)
					close(heredoc_pipe[1]);
				if (exp_delim)
					free(exp_delim);
				print_err(PROMPT, "heredoc pipe failed : ", exp_delim, NULL);
				return (ERR_HDOC);
			}
			err = heredoc_input_to_pipe(exp_delim, heredoc_pipe[1], &cmd_count, is_quoted, env);
			free(exp_delim);
			exp_delim = NULL;
			close(heredoc_pipe[1]);
			token_list->hdoc_fd = heredoc_pipe[0];
			if (err)
				return (err);
			if (g_signal == SIGINT)
				return (ERR_SUCCESS);
			token_list = token_list->next;
		}
	}
	return (ERR_SUCCESS);
}

void	shell_loop(struct s_data *data, int *status)
{
	char 	*input;
	int		cmd_count;

	*status = 0;
	cmd_count = 0;
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
		get_input(&input, data, *status, &cmd_count);
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
		if (heredocs(data->token_list, cmd_count, data->env))
			my_exit(2, &data->env, &input, &data->token_list_head); //TODO : le retour se fait dans fonction heredoc ?
		else if (g_signal == SIGINT)
			my_exit(SIGINT + 128,  &data->env, &input, &data->token_list_head);
		if (parser(&data->token_list))
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

	g_signal = 0;
	// loading();
	init_readline_signals();
	data.env = init_env_list(env);
	if (!data.env)
		return (print_err(PROMPT, PERR_MALLOC, NULL, NULL), 1);
	status = 0;
	shell_loop(&data, &status);
	exit_clean(&data);
	return(status);
	(void)ac;
	(void)av;
}





/*
*	heredoc
*	-> expansion dans heredoc
*	->
*	exit as command (+ args ?)
*	clean / valgrind / fds / norminette
*/

// TO FIX !!

// unset PATH
// -> ls 

// echo -n -nnnnnn -na -n

// echo test > out

// env should not print locale var

// unset first env

// ./minishell | ./minishell

// mkdir -> rm ../. -> pwd ou cd ..

// 
