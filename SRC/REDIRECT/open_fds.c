/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:56:24 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/23 17:34:36 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_open_error(t_exec *exec_list, struct s_exec_data *exec_data, int i)
{
	print_err(PROMPT, NULL, NULL, NULL);
	perror(exec_list->command->redir[i]);
	clean_data_close_fds(exec_data, NULL, 1);
	if (!exec_data->is_pipe && is_builtin(exec_list))
		return (1);
	if (exec_data->is_pipe && !exec_list->command->argv[0])
	{
		clean_exec_list(&exec_list);
		exit(0);
	}
	clean_exec_list(&exec_list);
	exit(1);
	return (0);
}

int	open_fd_out(int i, t_exec *exec_list, struct s_exec_data *exec_data)
{
	int	fd;

	if (exec_list->command->redir_kind[i] == OUT_APP)
	{
		fd = open(exec_list->command->redir[i], O_RDWR
				| O_APPEND | O_CREAT, 0777);
		if (fd == -1)
			return (handle_open_error(exec_list, exec_data, i), -1);
	}
	else
	{
		fd = open(exec_list->command->redir[i], O_RDWR
				| O_TRUNC | O_CREAT, 0777);
		if (fd == -1)
			return (handle_open_error(exec_list, exec_data, i), -1);
	}
	return (fd);
}

int	open_hdoc_fds(t_exec *exec_list, int *hdoc_index)
{
	int	fd;

	fd = exec_list->command->hdoc_fd[*hdoc_index];
	*hdoc_index += 1;
	return (fd);
}

int	open_fd_in(int i, int *h, t_exec *exec_list, struct s_exec_data *exec_data)
{
	int	fd;

	fd = -1;
	if (exec_list->command->redir_kind[i] == IN)
	{
		fd = open(exec_list->command->redir[i], O_RDONLY);
		if (fd == -1)
			return (handle_open_error(exec_list, exec_data, i), -1);
	}
	else if (exec_list->command->redir_kind[i] == HDOC)
	{
		fd = open_hdoc_fds(exec_list, h);
		if (fd == -1)
			return (handle_open_error(exec_list, exec_data, i), -1);
	}
	return (fd);
}

void	open_fds(t_exec *exec_list, int *fd_in, int *fd_out, struct s_exec_data *exec_data)
{
	int	i;
	int	h;

	i = 0;
	h = 0;
	while (exec_list->command->redir[i])
	{
		if (is_in_redirection(exec_list, i))
		{
			*fd_in = open_fd_in(i, &h, exec_list, exec_data);
			if (*fd_in == -1 && *fd_out != -1)
				close(*fd_out);
		}
		if (h > 0)
			exec_list->command->hdoc_fd[h - 1] = -1;
		if (is_out_redirection(exec_list, i))
		{
			*fd_out = open_fd_out(i, exec_list, exec_data);
			if (*fd_out == -1 && *fd_in != -1)
				close(*fd_in);
		}
			i++;
	}
}
