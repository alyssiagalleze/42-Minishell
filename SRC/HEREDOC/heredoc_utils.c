/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:07:17 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/29 13:07:47 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_hdoc_data(struct s_heredoc *hdoc_data)
{
	hdoc_data->heredoc_fds[0] = -1;
	ft_close(&hdoc_data->heredoc_fds[1]);
	ft_close(&hdoc_data->heredoc_fds[2]);
	hdoc_data->exp_delim = ft_free(hdoc_data->exp_delim);
}

void	init_hdoc_data(struct s_heredoc *hdoc_data)
{
	hdoc_data->exp_delim = NULL;
	hdoc_data->heredoc_fds[0] = -1;
	hdoc_data->heredoc_fds[1] = -1;
	hdoc_data->heredoc_fds[2] = -1;
	hdoc_data->is_quoted = FALSE;
}

t_token	*skip_until_heredoc_delim(t_token *token_list)
{
	while (token_list && token_list->kind != HDOC)
		token_list = token_list->next;
	if (token_list)
		token_list = token_list->next;
	return (token_list);
}

void	heredocs_display_header(char *delim)
{
	write(1, HDOC_HEADER, 11);
	write(1, delim, ft_strlen(delim));
	write(1, "\n", 1);
}
