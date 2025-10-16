/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:33:55 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/15 18:59:34 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(int pipefds[2], int saved_stds[2])
{
	if (pipefds)
	{
		if (pipefds[0] != -1)
			close(pipefds[0]);
		if (pipefds[1] != -1)
			close(pipefds[1]);
	}
	if (saved_stds)
	{
		printf("-> in close fds\n");
		if (saved_stds[0] != -1)
			close(saved_stds[0]);
		if (saved_stds[1] != -1)
			close(saved_stds[1]);
	}
}

int	handle_open_error(t_exec *exec_list, int fd, int is_single)
{
	perror(exec_list->command->redir[0]);
	close(fd);
	if (is_single)
		return (-1);
	exit(1);
	return (0);
}

int	open_fd_out(int i, t_exec *exec_list, int is_single)
{
	int	fd;

	if (exec_list->command->redir_kind[i] == OUT_APP)
	{
		fd = open(exec_list->command->redir[i], O_RDWR
				| O_APPEND | O_CREAT, 0777);
		if (fd == -1)
			return (handle_open_error(exec_list, fd, is_single));
	}
	else
	{
		fd = open(exec_list->command->redir[i], O_RDWR
				| O_TRUNC | O_CREAT, 0777);
		if (fd == -1)
			return (handle_open_error(exec_list, fd, is_single));
	}
	return (fd);
}

int	open_fd_in(int i, t_exec *exec_list, int is_single)
{
	int	fd;

	fd = open(exec_list->command->redir[i], O_RDONLY);
	if (fd == -1)
		return (handle_open_error(exec_list, fd, is_single));
	return (fd);
}

void	open_fds(t_exec *exec_list, int *fd_in, int *fd_out)
{
	int	i;

	i = 0;
	while (exec_list->command->redir[i])
	{
		if (exec_list->command->redir_kind[i] == IN)
		{
			*fd_in = open_fd_in(i, exec_list, 0);
			if (*fd_in == -1)
				close_and_exit(*fd_in, *fd_out, NULL, 1);
		}
		if (exec_list->command->redir_kind[i] == OUT ||
			exec_list->command->redir_kind[i] == OUT_APP)
		{
			*fd_out = open_fd_out(i, exec_list, 0);
			if (*fd_out == -1)
				close_and_exit(*fd_in, *fd_out, NULL, 1);
		}
			i++;
	}
}

//  il faut ouvrir les fds dans l ordre mais il 

// ici il faut exit apres perror

void	init_std_fds(struct s_data *subshell_data)
{
	printf("-> in init std fds\n");
	subshell_data->std_fds[0] = dup(STDIN_FILENO);
	if (subshell_data->std_fds[0] == -1)
		return (perror("dup"));
	subshell_data->std_fds[1] = dup(STDOUT_FILENO);
	if (subshell_data->std_fds[1] == -1)
		return (perror("dup"));
}
