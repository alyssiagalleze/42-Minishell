/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 14:02:50 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/24 11:36:03 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_copy_until_dollar(char **input, char **new_input)
{
	int		len;
	char	*substr;

	len = 0;
	while ((*input)[len] && (*input)[len] != '$')
		len++;
	substr = extract_string(*input, len);
	if (substr == NULL)
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	*new_input = str_append(*new_input, substr);
	if (*new_input == NULL)
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	*input += len;
	return (ERR_SUCCESS);
}

int	heredoc_get_var_name_length(const char *input)
{
	int	len;

	len = 1;
	if (!is_char_in_string(input[len], "0123456789", FALSE, FALSE))
		while (is_expandable_char(input[len]))
			len++;
	return (len);
}

int	heredoc_expand_get_var_value(
	char **input, int len, t_env *env, char **substr)
{
	t_env	*var;

	if (len == 1)
	{
		*substr = ft_strdup("$");
		if (*substr == NULL)
			return (ERR_MALLOC);
	}
	else
	{
		*substr = extract_string(*input + 1, len - 1);
		if (*substr == NULL)
			return (ERR_MALLOC);
		var = var_exists(&env, *substr);
		*substr = ft_free(*substr);
		if (var == NULL)
			return (ERR_SUCCESS);
		*substr = ft_strdup(var->var_value);
		if (*substr == NULL)
			return (ERR_MALLOC);
	}
	return (ERR_SUCCESS);
}

int	heredoc_expand_input_var(char **input, char **new_input, t_env *env)
{
	int		len;
	char	*substr;

	len = heredoc_get_var_name_length(*input);
	if (heredoc_expand_get_var_value(input, len, env, &substr))
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	if (substr)
	{
		*new_input = str_append(*new_input, substr);
		if (*new_input == NULL)
		{
			print_err(PROMPT, PERR_MALLOC, NULL, NULL);
			return (ERR_MALLOC);
		}
	}
	*input += len;
	return (ERR_SUCCESS);
}

int	heredoc_expand_input(char **input, t_env *env)
{
	char	*input_save;
	char	*new_input;

	input_save = *input;
	new_input = NULL;
	while (**input)
	{
		if (**input != '$')
		{
			if (heredoc_copy_until_dollar(input, &new_input))
				return (ft_free(input_save), ft_free(new_input), ERR_MALLOC);
		}
		else if (**input == '$')
		{
			if (heredoc_expand_input_var(input, &new_input, env))
				return (ft_free(input_save), ft_free(new_input), ERR_MALLOC);
		}
	}
	input_save = ft_free(input_save);
	*input = new_input;
	return (ERR_SUCCESS);
}

void	heredoc_inform_eof(int *cmd_count, char *exp_delim)
{
	char	*line_count_str;

	*cmd_count += 1;
	line_count_str = ft_itoa(*cmd_count);
	if (line_count_str)
	{
		print_err(PERR_HDOC, line_count_str, PERR_EOF, NULL);
		free(line_count_str);
	}			
	else
		print_err(PERR_HDOC_EOF, exp_delim, "/')\n", NULL);
}

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
	while (input[i])
		i++;
	i--;
	if (input[i] == '\n')
		input[i] = '\0';
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
		{
			*err = ERR_SUCCESS;
			return (ERR_SUCCESS);
		}
		if (!input)
		{
			heredoc_inform_eof(cmd_count, hdoc_data->exp_delim);
			// close(hdoc_data->heredoc_fds[1]); // fix les fd leack avec ctrl D mais error dup2
			// close(hdoc_data->heredoc_fds[0]);
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

void	heredocs_display_header(char *delim)
{
	write(1, HDOC_HEADER, 11);
	write(1, delim, ft_strlen(delim));
	write(1, "\n", 1);
}

int	heredoc_skip_quotes(
	char *delim, char *new_delim, int iter[3], int *is_quoted)
{
	char	c;

	if (delim[iter[0]] == '\'' || delim[iter[0]] == '"')
	{
		c = delim[iter[0]];
		*is_quoted = TRUE;
		iter[0]++;
		while (iter[0] < iter[2] && delim[iter[0]] != c)
		{
			new_delim[iter[1]] = delim[iter[0]];
			iter[0]++;
			iter[1]++;
		}
		if (iter[0] >= iter[2])
		{
			print_err(PROMPT, PERR_STX_HDOC, NULL, NULL);
			new_delim = ft_free(new_delim);
			return (ERR_AMBIG);
		}
		iter[0]++;
	}
	return (ERR_SUCCESS);
}

void	heredoc_copy_delim_until_quote(
	char *delim, char **new_delim, int iter[3])
{
	while (iter[0] < iter[2]
		&& delim[iter[0]] != '\'' && delim[iter[0]] != '"')
	{
		(*new_delim)[iter[1]] = delim[iter[0]];
		iter[0]++;
		iter[1]++;
	}
}

int	heredoc_unquote_delim(char *delim, struct s_heredoc *hdoc_data, int *err)
{
	int		iter[3];

	iter[0] = 0;
	iter[1] = 0;
	iter[2] = ft_strlen(delim);
	hdoc_data->exp_delim = malloc(sizeof(*delim) * (iter[2] + 1));
	if (!hdoc_data->exp_delim)
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		*err = ERR_MALLOC;
		return (*err);
	}
	while (iter[0] < iter[2])
	{
		*err = heredoc_skip_quotes(
				delim, hdoc_data->exp_delim, iter, &hdoc_data->is_quoted);
		if (*err)
			return (*err);
		heredoc_copy_delim_until_quote(delim, &hdoc_data->exp_delim, iter);
	}
	hdoc_data->exp_delim[iter[1]] = '\0';
	*err = ERR_SUCCESS;
	return (*err);
}

t_token	*skip_until_heredoc_delim(t_token *token_list)
{
	while (token_list && token_list->kind != HDOC)
		token_list = token_list->next;
	if (token_list)
		token_list = token_list->next;
	return (token_list);
}

int	heredoc_fds_management(struct s_heredoc *hdoc_data, int	*err)
{
	hdoc_data->heredoc_fds[0] = -1;
	hdoc_data->heredoc_fds[1] = -1;
	hdoc_data->heredoc_fds[2] = dup(STDIN_FILENO);
	if (pipe(hdoc_data->heredoc_fds) || hdoc_data->heredoc_fds[2] == -1)
	{
		ft_close(&hdoc_data->heredoc_fds[0]);
		ft_close(&hdoc_data->heredoc_fds[1]);
		ft_close(&hdoc_data->heredoc_fds[2]);
		print_err(PROMPT, PERR_HDOC_FDS, hdoc_data->exp_delim, NULL);
		hdoc_data->exp_delim = ft_free(hdoc_data->exp_delim);
		*err = ERR_HDOC;
		return (*err);
	}
	*err = ERR_SUCCESS;
	return (*err);
}

void	init_hdoc_data(struct s_heredoc *hdoc_data)
{
	hdoc_data->exp_delim = NULL;
	hdoc_data->heredoc_fds[0] = -1;
	hdoc_data->heredoc_fds[1] = -1;
	hdoc_data->heredoc_fds[2] = -1;
	hdoc_data->is_quoted = FALSE;
}

int	heredoc_anticipated_stop(struct s_heredoc *hdoc_data, int err)
{
	if (g_signal == SIGINT
		&& dup2(hdoc_data->heredoc_fds[2], STDIN_FILENO) == -1)
	{
		print_err(PROMPT, PERR_HDOC_FDS, hdoc_data->exp_delim, NULL);
		err = ERR_HDOC;
	}
	hdoc_data->exp_delim = ft_free(hdoc_data->exp_delim);
	ft_close(&hdoc_data->heredoc_fds[0]);
	ft_close(&hdoc_data->heredoc_fds[1]);
	ft_close(&hdoc_data->heredoc_fds[2]);
	return (err);
}

void	reset_hdoc_data(struct s_heredoc *hdoc_data)
{
	hdoc_data->heredoc_fds[0] = -1;
	ft_close(&hdoc_data->heredoc_fds[1]);
	ft_close(&hdoc_data->heredoc_fds[2]);
	hdoc_data->exp_delim = ft_free(hdoc_data->exp_delim);
}

int	heredocs(t_token *token_list, int cmd_count, t_env *env)
{
	struct s_heredoc	hdoc_data;
	int					err;

	init_heredoc_signals();
	init_hdoc_data(&hdoc_data);
	while (token_list)
	{
		token_list = skip_until_heredoc_delim(token_list);
		if (!token_list)
			break ;
		if (heredoc_unquote_delim(token_list->str, &hdoc_data, &err))
			return (err);
		heredocs_display_header(hdoc_data.exp_delim);
		if (heredoc_fds_management(&hdoc_data, &err)
			|| heredoc_input_to_pipe(&hdoc_data, &cmd_count, env, &err)
			|| g_signal == SIGINT)
			return (heredoc_anticipated_stop(&hdoc_data, err));
		token_list->hdoc_fd = hdoc_data.heredoc_fds[0];
		reset_hdoc_data(&hdoc_data);
	}
	return (ERR_SUCCESS);
}
