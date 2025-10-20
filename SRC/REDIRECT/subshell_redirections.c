/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:59:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/20 14:19:52 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sub_redirect_out(t_exec *exec_list, int pipefds[2], int *fd_out)
{
	if (out_redirections(exec_list) == TRUE)
	{
		if (dup2(*fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 fd_out"), 1);
		close(*fd_out);
	}
    // else if (!exec_list->next)
    // {
	// 	saved_std_out = -1;
	// 	if (dup2(saved_std_out, STDOUT_FILENO) == -1)
	// 		// exit(1);
	// 	close(saved_std_out);
    // }
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
	printf("--> pipefds in : %d, out : %d\n", pipefds[0], pipefds[1]);
	int fd_in;
	int fd_out;

	if (redirect_in(exec_list, &fd_in, prev_fd))
		return (1);
	if (sub_redirect_out(exec_list, pipefds, &fd_out))
		return (1);
	return (0);	
}
