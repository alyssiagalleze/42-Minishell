/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:59:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 14:35:57 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sub_redirect_out(int pipefds[2])
{
	if (pipefds[1] != -1)
	{
		if (dup2(pipefds[1], STDOUT_FILENO) == -1)
			return (perror("dup2 sub pipe write"), 1);
		close(pipefds[1]);
	}
	return (0);
}

int	sub_redirect_fds(t_exec *exec_list, int pipefds[2], int prev_fd)
{
	int	fd_in;

	if (redirect_in(exec_list, &fd_in, prev_fd))
		return (1);
	if (sub_redirect_out(pipefds))
		return (1);
	return (0);
}
