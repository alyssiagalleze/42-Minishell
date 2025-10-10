/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:12:43 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/10 15:23:27 by agalleze         ###   ########.fr       */
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
			return (print_err(PROMPT, exec_list->command->redir[i], ": ", "No such file of directory\n"), -1);
	}	
	else 
	{
		fd = open(exec_list->command->redir[i], O_RDWR | O_TRUNC | O_CREAT, 0777);
		if (fd == -1)
			return (print_err(PROMPT, exec_list->command->redir[i], ": ", "No such file of directory\n"),perror(exec_list->command->redir[i]), -1);
	}
	return (fd);
}

int	open_fd_in(int i, t_exec *exec_list)
{
	int	fd;

	fd = open(exec_list->command->redir[i], O_RDONLY);
	if ( fd == -1)
		return (print_err(PROMPT, exec_list->command->redir[i], ": ", "No such file of directory\n"), -1);
	return (fd);
}

int	*open_fds(t_exec *exec_list)
{
	int i;
	int	fds[2];
	
	i = 0;
	fds[0] = -1;
	fds[1] = -1;
	while (exec_list->command->redir[i])
	{
		if (exec_list->command->redir_kind == IN)
			fds[0] = open_fd_in(i, exec_list);
		if (exec_list->command->redir_kind == OUT ||
			exec_list->command->redir_kind == OUT_APP)
			fds[1] = open_fd_out(i, exec_list);
			i++;
	}
	return (fds);
}
int	redirect_fds(t_exec *exec_list, int pipefds[2], int prev_fd)
{
	int prev;
	int *redir_fds;

	if (exec_list->command->redir)
	{
		redir_fds = open_fds(exec_list);
		if (redir_fds[0] == -1 || redir_fds[0] == -1)
			return (1);
	}
	if (redir_fds == -1)
	
    prev = exec_list->command->prev_fd;
    if (prev != -1)
    {
		// ce n est pas la premiere commande
        if (dup2(prev, STDIN_FILENO) == -1)
            exit(1);
        close(prev);
    }
    if (exec_list->next)
    {
        if (dup2(pipefds[1], STDOUT_FILENO) == -1)
            exit(1);
        close(pipefds[1]);
        close(pipefds[0]);
    }
	if (dup2(redir_fds[0], STDIN_FILENO) == -1)
		return (print_err(PROMPT, "redirections: ", exec_list->command->argv[0], "duplicate STD in failed\n"), 1);
	if (dup2(redir_fds[1], STDIN_FILENO) == -1)
		return (print_err(PROMPT, "redirections: ", exec_list->command->argv[0], "duplicate STD out failed\n"), 1);
	return (0);	
}

// int	here_doc_input(int i, t_exec *exec_list)
// {
// 	int		pipefds[2];
// 	char	*line;
	
// 	if (pipe(pipefds) == -1)
// 		return (print_err("heredoc pipe : pipe creation failed.\n", NULL, NULL, NULL), -1);

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
