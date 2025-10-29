/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:55:02 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/28 16:16:03 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	in_redirections(t_exec *exec_list)
{
	int	i;

	i = 0;
	while (exec_list->command->redir[i])
	{
		if (exec_list->command->redir_kind[i] == IN
			|| exec_list->command->redir_kind[i] == HDOC)
			return (1);
		i++;
	}
	return (0);
}

int	out_redirections(t_exec *exec_list)
{
	int	i;

	i = 0;
	while (exec_list->command->redir[i])
	{
		if (exec_list->command->redir_kind[i] == OUT
			|| exec_list->command->redir_kind[i] == OUT_APP)
			return (1);
		i++;
	}
	return (0);
}

int	redirect_in(t_exec *exec_list, int *fd_in, int prev_fd)
{
	if (in_redirections(exec_list) == TRUE)
	{
		if (dup2(*fd_in, STDIN_FILENO) == -1)
			return (perror("dup2 fd_in"), 1);
		ft_close(fd_in);
	}
	else if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			return (perror("dup2 prev_fd"), 1);
		ft_close(&prev_fd);
	}
	return (0);
}

int	redirect_out(t_exec *exec_list, int *fd_out, int pipefd_out)
{
	if (out_redirections(exec_list) == TRUE)
	{
		if (dup2(*fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 fd_out"), 1);
		close(*fd_out);
	}
	else if (exec_list->next)
	{
		if (dup2(pipefd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 pipe write"), 1);
		close(pipefd_out);
	}
	return (0);
}

int	redirect_fds(t_exec *exec_list,
	int pipefds[2], struct s_exec_data *exec_data)
{
	static int	fd_in = -1;
	static int	fd_out = -1;

	if (exec_list->command->redir[0])
		open_fds(exec_list, &fd_in, &fd_out, exec_data);
	if (redirect_in(exec_list, &fd_in, exec_data->prev_fd))
		return (1);
	if (redirect_out(exec_list, &fd_out, pipefds[1]))
		return (1);
	if (pipefds[0] != -1)
		close(pipefds[0]);
	if (pipefds[1] != -1)
		close(pipefds[1]);
	return (0);
}
