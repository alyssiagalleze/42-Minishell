/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:22:13 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/19 17:13:59 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_buffer(char *buffer, const char *fill)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (buffer == NULL || fill == NULL)
		return ;
	while (buffer[i])
		i ++;
	while (fill[j])
	{
		buffer[i + j] = fill[j];
		j ++;
	}
}

void	print_err(
	const char *str1, const char *str2, const char *str3, const char *str4)
{
	char	error_buf[MAX_ERROR_LEN];
	int		len;
	int		i;

	len = ft_strlen(str1) + ft_strlen(str2)
		+ ft_strlen(str3) + ft_strlen(str4) + 1;
	i = -1;
	while (++i < MAX_ERROR_LEN)
	{
		error_buf[i] = 0;
	}
	if (len > MAX_ERROR_LEN)
	{
		fill_buffer(error_buf, PROMPT);
		fill_buffer(error_buf, ERROR_TOO_LONG);
		write(2, error_buf, strlen(PROMPT) + strlen(ERROR_TOO_LONG) + 1);
	}
	else
	{
		fill_buffer(error_buf, str1);
		fill_buffer(error_buf, str2);
		fill_buffer(error_buf, str3);
		fill_buffer(error_buf, str4);
		write(2, error_buf, len);
	}
}
