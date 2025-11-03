/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:56:24 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 15:55:20 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_fd_in(int i, int *h, t_exec *exec_list, struct s_exec_data *exec_data)
{
	int	fd;

	fd = -1;
	if (exec_list->command->redir_kind[i] == IN)
	{
		fd = open(exec_list->command->redir[i], O_RDONLY);
		if (fd == -1)
			return (handle_open_error(exec_list, exec_data, i));
	}
	else if (exec_list->command->redir_kind[i] == HDOC)
	{
		fd = open_hdoc_fds(exec_list, h);
		if (fd == -1)
			return (handle_open_error(exec_list, exec_data, i));
	}
	return (fd);
}

void	close_prev_fds(int prev, struct s_exec_data *exec_data)
{
	if (prev != -1)
		ft_close(&prev);
	if (exec_data && exec_data->prev_fd != -1)
		ft_close(&exec_data->prev_fd);
}

void	init_values(int *i, int *h, int *prev_in, int *prev_out)
{
	*i = 0;
	*h = 0;
	*prev_in = -1;
	*prev_out = -1;
}

int	open_fds(
	t_exec *exec_list, int *fd_in,	int *fd_out, struct s_exec_data *exec)
{
	int	i;
	int	h;
	int	prev_in;
	int	prev_out;

	init_values(&i, &h, &prev_in, &prev_out);
	while (exec_list->command->redir[i])
	{
		if (is_in_redirection(exec_list, i))
		{
			*fd_in = open_fd_in(i, &h, exec_list, exec);
			if (*fd_in == -1)
				return (close_prev_fds(prev_in, exec), ft_close(fd_out), -1);
			prev_in = *fd_in;
		}
		if (is_out_redirection(exec_list, i))
		{
			*fd_out = open_fd_out(i, exec_list, exec);
			if (*fd_out == -1)
				return (close_prev_fds(prev_out, NULL), ft_close(fd_in), -1);
			prev_out = *fd_out;
		}
			i++;
	}
	return (0);
}
