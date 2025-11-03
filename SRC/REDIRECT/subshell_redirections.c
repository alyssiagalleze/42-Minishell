/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:59:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 12:49:25 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO
int	sub_redirect_out(t_exec *exec_list, int pipefds[2], int *fd_out)
{
	if (out_redirections(exec_list) == TRUE)
	{
		if (dup2(*fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 fd_out"), 1);
		close(*fd_out);
	}
	else if (pipefds[1] != -1)
	{
		if (dup2(pipefds[1], STDOUT_FILENO) == -1)
			// exit(1);
		close(pipefds[1]);
	}
	return (0);
}

int	sub_redirect_fds(t_exec *exec_list, int pipefds[2], int prev_fd)
{
	int	fd_in;
	int	fd_out;

	if (redirect_in(exec_list, &fd_in, prev_fd))
		return (1);
	if (sub_redirect_out(exec_list, pipefds, &fd_out))
		return (1);
	return (0);
}
