/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:33:55 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/13 11:34:59 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_fd_out(int i, t_exec *exec_list)
{
	int fd;

	if (exec_list->command->redir_kind[i] == OUT_APP)
	{
		fd = open(exec_list->command->redir[i], O_RDWR | O_APPEND | O_CREAT, 0777);
		if (fd == -1)
			return (perror(exec_list->command->redir[i]), -1);
	}	
	else 
	{
		fd = open(exec_list->command->redir[i], O_RDWR | O_TRUNC | O_CREAT, 0777);
		if (fd == -1)
			return (perror(exec_list->command->redir[i]), -1);
	}
	return (fd);
}

int	open_fd_in(int i, t_exec *exec_list)
{
	int	fd;

	fd = open(exec_list->command->redir[i], O_RDONLY);
	if ( fd == -1)
		return (perror(exec_list->command->redir[i]), -1);
	return (fd);
}

void	open_fds(t_exec *exec_list, int *fd_in, int *fd_out)
{
	printf("in open fds \n");
	int i;
	
	i = 0;
	while (exec_list->command->redir[i])
	{
		if (exec_list->command->redir_kind[i] == IN)
			*fd_in = open_fd_in(i, exec_list);
				if (*fd_in == -1)
					break;
		if (exec_list->command->redir_kind[i] == OUT ||
			exec_list->command->redir_kind[i] == OUT_APP)
			*fd_out = open_fd_out(i, exec_list);
				if (*fd_out == -1)
					break;
			i++;
	}
}
