/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:03:46 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/22 18:11:51 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strcat(char *str1, char *str2)
{
	char	*join_str;
	int		j;
	int		i;

	i = 0;
	while (str1[i])
		i ++;
	j = 0;
	while (str2[j])
		j ++;
	join_str = malloc(i + j + 1);
	if (join_str != NULL)
	{
		join_str[i + j] = '\0';
		while (j-- > 0)
			join_str[i + j] = str2[j];
		while (i-- > 0)
			join_str[i] = str1[i];
	}
	free(str1);
	free(str2);
	return (join_str);
}

void	*ft_free_all(char *result, t_buff **buffer)
{
	free(result);
	result = NULL;
	free(*buffer);
	*buffer = NULL;
	return (NULL);
}

t_buff	*ft_initialize_buffer(t_buff *buffer)
{
	int		i;

	i = 0;
	if (buffer == NULL)
		buffer = malloc(sizeof(t_buff));
	if (buffer == NULL)
		return (NULL);
	while (i <= BUFFER_SIZE)
		buffer->initial_pos[i++] = '\0';
	buffer->current_pos = (char *)buffer->initial_pos;
	return (buffer);
}

int	ft_get_line_len(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\n')
		{
			return (++i);
		}
		i ++;
	}
	return (0);
}
