/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:04:16 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/22 18:10:48 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_add_to_result(char *buffer, char *prev_result)
{
	char	*new_result;

	new_result = NULL;
	if (prev_result == NULL)
	{
		new_result = ft_strdup(buffer);
		return (new_result);
	}
	new_result = ft_strdup(buffer);
	if (new_result == NULL)
	{
		free(prev_result);
		return (NULL);
	}
	new_result = ft_strcat(prev_result, new_result);
	return (new_result);
}

char	*ft_found_line(char **result, t_buff **buffer, int line_len)
{
	*result = ft_add_to_result((*buffer)->current_pos, *result);
	if (*result == NULL)
		return (ft_free_all(*result, buffer));
	(*buffer)->current_pos += line_len;
	if ((*buffer)->current_pos[0] == '\0')
	{
		free(*buffer);
		*buffer = NULL;
	}
	return (*result);
}

void	ft_check_buffer(char **result, t_buff **buffer, int *line_ready)
{
	int	line_len;

	line_len = ft_get_line_len((*buffer)->current_pos);
	if (line_len)
	{
		*line_ready = 1;
		*result = (ft_found_line(result, buffer, line_len));
		return ;
	}
	*result = ft_add_to_result((*buffer)->current_pos, *result);
	if (result == NULL)
	{
		*line_ready = 1;
		*result = (ft_free_all(*result, buffer));
		return ;
	}
	(*buffer)->current_pos = &((*buffer)->initial_pos)[BUFFER_SIZE];
}

void	ft_fill_buffer(int fd, char **result, t_buff **buffer, int *line_ready)
{
	int	read_amount;

	*buffer = ft_initialize_buffer(*buffer);
	if (*buffer == NULL)
	{
		*line_ready = 1;
		*result = ft_free_all(*result, buffer);
		return ;
	}
	read_amount = read(fd, (*buffer)->initial_pos, BUFFER_SIZE);
	if (read_amount < 0 || (*buffer)->initial_pos[0] == '\0')
	{
		*line_ready = 1;
		if (!((*buffer)->initial_pos[0] == '\0'))
			ft_free_all(*result, buffer);
		else
			ft_free_all(NULL, buffer);
	}
}

char	*get_next_line(int fd)
{
	static t_buff	*buffer;
	char			*result;
	int				line_ready;

	result = NULL;
	line_ready = 0;
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, "", 0) == -1)
		return (ft_free_all(result, &buffer));
	if (buffer != NULL && buffer->current_pos[0] != '\n')
		ft_check_buffer(&result, &buffer, &line_ready);
	while (!(line_ready))
	{
		ft_fill_buffer(fd, &result, &buffer, &line_ready);
		if (!(line_ready))
			ft_check_buffer(&result, &buffer, &line_ready);
	}
	return (result);
}
