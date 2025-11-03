/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 14:02:50 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 10:51:48 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		print_err(PROMPT, E_HDOC_FDS, hdoc_data->exp_delim, NULL);
		hdoc_data->exp_delim = ft_free(hdoc_data->exp_delim);
		*err = ERR_HDOC;
		return (*err);
	}
	*err = ERR_SUCCESS;
	return (*err);
}

int	heredoc_anticipated_stop(struct s_heredoc *hdoc_data, int err)
{
	if (g_signal == SIGINT
		&& dup2(hdoc_data->heredoc_fds[2], STDIN_FILENO) == -1)
	{
		print_err(PROMPT, E_HDOC_FDS, hdoc_data->exp_delim, NULL);
		err = ERR_HDOC;
	}
	hdoc_data->exp_delim = ft_free(hdoc_data->exp_delim);
	ft_close(&hdoc_data->heredoc_fds[0]);
	ft_close(&hdoc_data->heredoc_fds[1]);
	ft_close(&hdoc_data->heredoc_fds[2]);
	return (err);
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
