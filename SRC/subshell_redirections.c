/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 18:59:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/17 19:36:18 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sub_redirect_out(t_exec *exec_list, int pipefds[2], int *fd_out, int saved_std_out)
{
	printf("--> pipefds in : %d, out : %d\n", pipefds[0], pipefds[1]);
	if (out_redirections(exec_list) == TRUE)
	{
		// printf("out redirect ?\n");
		// printf("fd_out : %d\n", *fd_out);
		if (dup2(*fd_out, STDOUT_FILENO) == -1)
			return (perror("dup2 fd_out"), 1);
		close(*fd_out);
	}
    else if (!exec_list->next)
    {
		if (dup2(saved_std_out, STDOUT_FILENO) == -1)
			// exit(1);
		close(saved_std_out);
    }
	else if (pipefds[1] != -1)
	{
		if (dup2(pipefds[1], STDOUT_FILENO) == -1)
			// exit(1);
		close(pipefds[1]);
	}
	return (0);
}

int	sub_redirect_fds(t_exec *exec_list, int pipefds[2], int prev_fd, int saved_stdout)
{
	// printf("in redirect fds for redir : %s\n", exec_list->command->redir[0]);
	// printf("--> pipefds in : %d, out : %d\n", pipefds[0], pipefds[1]);
	
	int fd_in;
	int fd_out;

	// if (exec_list->command->redir[0] != NULL)
	// {
	// 	open_fds(exec_list, &fd_in, &fd_out);
	// 	if (fd_in == -1)
	// 		return (1);
	// 	if (fd_out == -1)
	// 		return (1);
	// 	printf("updated fd_in value : %d\n", fd_in);
	// 	printf("updated fd_out value : %d\n", fd_out);
	// }
	if (redirect_in(exec_list, &fd_in, prev_fd))
		return (1);
	if (sub_redirect_out(exec_list, pipefds, &fd_out, saved_stdout))
		return (1);
	return (0);	
}
