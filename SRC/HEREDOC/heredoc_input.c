/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:04:03 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 10:51:48 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_expand_write_input(
	char *input, struct s_heredoc *hdoc_data, t_env *env, int *cmd_count)
{
	int	err;

	if (!is_str_empty_or_null(input) && !(hdoc_data->is_quoted))
	{
		err = heredoc_expand_input(&input, env);
		if (err)
		{
			input = ft_free(input);
			return (err);
		}
	}
	write(hdoc_data->heredoc_fds[1], input, ft_strlen(input));
	write(hdoc_data->heredoc_fds[1], "\n", 1);
	input = ft_free(input);
	*cmd_count += 1;
	return (ERR_SUCCESS);
}

void	heredoc_remove_newline(char *input)
{
	int	i;

	i = 0;
	if (!input)
		return ;
	while (input[i])
		i++;
	i--;
	if (input[i] == '\n')
		input[i] = '\0';
}

void	heredoc_inform_eof(int *cmd_count, char *exp_delim)
{
	char	*line_count_str;

	*cmd_count += 1;
	line_count_str = ft_itoa(*cmd_count);
	if (line_count_str)
	{
		print_err(PROMPT, E_HDOC, line_count_str, E_EOF);
		free(line_count_str);
	}			
	else
		print_err(PROMPT, E_HDOC_EOF, exp_delim, "/')\n");
}

int	heredoc_input_to_pipe(
	struct s_heredoc *hdoc_data, int *cmd_count, t_env *env, int *err)
{
	char	*input;

	input = NULL;
	while (1)
	{
		write(1, HDOC_PROMPT, 2);
		input = get_next_line(STDIN_FILENO);
		if (g_signal == SIGINT)
			break ;
		if (!input)
		{
			write(1, "\n", 1);
			heredoc_inform_eof(cmd_count, hdoc_data->exp_delim);
			break ;
		}
		heredoc_remove_newline(input);
		if (str_cmp(input, hdoc_data->exp_delim, FALSE))
			break ;
		*err = heredoc_expand_write_input(input, hdoc_data, env, cmd_count);
		if (*err)
			return (*err);
	}
	input = ft_free(input);
	*err = ERR_SUCCESS;
	return (*err);
}
