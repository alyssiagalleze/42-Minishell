/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_delim.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:01:15 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 10:51:48 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			print_err(PROMPT, E_STX_HDOC, NULL, NULL);
			new_delim = ft_free(new_delim);
			return (ERR_AMBIG);
		}
		iter[0]++;
	}
	return (ERR_SUCCESS);
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
		print_err(PROMPT, E_MALLOC, NULL, NULL);
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
