/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:04:22 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 10:50:08 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif
// HEADERS

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

// STRUCTS

typedef struct s_buff
{
	char	initial_pos[BUFFER_SIZE + 1];
	char	*current_pos;
}	t_buff;

// PROTOTYPES

/* get_next_line.c */

t_buff	*ft_initialize_buffer(t_buff *buffer);
char	*ft_add_to_result(char *buffer, char *prev_result);
char	*ft_found_line(char **result, t_buff **buffer, int line_len);
void	ft_check_buffer(char **result, t_buff **buffer, int *line_ready);
char	*get_next_line(int fd);

/* get_next_line_utils.c */

char	*ft_strcat(char *str1, char *str2);
void	*ft_free_all(char *result, t_buff **buffer);
int		ft_get_line_len(char *buffer);
char	*ft_strdup(const char *s);

#endif