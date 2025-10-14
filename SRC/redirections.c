/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:12:43 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/14 11:49:30 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int in_redirections(t_exec *exec_list)
{
	int	i;

	i = 0;
	while (exec_list->command->redir[i])
	{
		if (exec_list->command->redir_kind[i] == IN || exec_list->command->redir_kind[i] == HDOC)
			return (1);
		i++;
	}
	return (0);
}

int	out_redirections(t_exec *exec_list)
{
	int i;

	i = 0;
	while (exec_list->command->redir[i])
	{
		printf("-------------------redir : %s\n", exec_list->command->redir[i]);
		if (exec_list->command->redir_kind[i] == OUT || exec_list->command->redir_kind[i] == OUT_APP)
			return (1);
		i++;
	}
	return (0);	
}

int	redirect_in(t_exec *exec_list, int *fd_in, int prev_fd)
{
	printf("in redirect in\n");
	if (in_redirections(exec_list) == TRUE)
	{
		printf("in redirect ?\n");
		if (dup2(*fd_in, STDIN_FILENO) == -1)
			return (perror("dup2 fd_in"), 1);
		close(*fd_in);
	}
	else if (prev_fd != -1)
    {
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			return (perror("dup2 prev_fd"), 1);
		close(prev_fd);
    }
	return (0);
}

int	redirect_out(t_exec *exec_list, int *fd_out, int pipefd_out)
{
	if (out_redirections(exec_list) == TRUE)
	{
		printf("out redirect ?\n");
		printf("fd_out : %d\n", *fd_out);
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

int	redirect_fds(t_exec *exec_list, int pipefds[2], int prev_fd)
{
	printf("in redirect fds for cmd : %s\n", exec_list->command->argv[0]);
	static int fd_in = -1;
	static int fd_out = -1;

	if (exec_list->command->redir[0])
	{
		open_fds(exec_list, &fd_in, &fd_out);
		printf("updated fd_in value : %d\n", fd_in);
		printf("updated fd_out value : %d\n", fd_out);
	}
	if (redirect_in(exec_list, &fd_in, prev_fd))
		return (1);
	if (redirect_out(exec_list, &fd_out, pipefds[1]))
		return (1);
	return (0);	
}

// int	here_doc_input(int i, t_exec *exec_list)
// {
// 	int		pipefds[2];
// 	char	*line;
// 
// 	if (pipe(pipefds) == -1)
// 		return (print_err("heredoc pipe : pipe creation failed.\n", NULL, NULL, NULL), -1);
// 
// 	line = NULL;
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			print_err(PROMPT, "warning: here-document (at line .. )delimited by end-of-file (wanted '", exec_list->command->redir_in[i], "')\n" );
// 			break;
// 		}
// 		if (str_ncmp(line, exec_list->command->redir_in[i], ft_strlen(exec_list->command->redir_in[i]) - 3, FALSE) == TRUE)
// 			break;
// 		write(pipefds[1], line, ft_strlen(line));
// 		write(pipefds[1], "\n", 1);
// 		free(line);
// 	}
// 	close(pipefds[1]);
// 	return (pipefds[0]);
// }
