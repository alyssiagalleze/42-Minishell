/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:33:55 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/14 11:46:18 by agalleze         ###   ########.fr       */
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
		{
			perror(exec_list->command->redir[i]);
			exit(1);
		}
	}	
	else 
	{
		fd = open(exec_list->command->redir[i], O_RDWR | O_TRUNC | O_CREAT, 0777);
		printf("-> open fd out for file %s, got fd : %d\n", exec_list->command->redir[i], fd);
		if (fd == -1)
		{
			printf("je passe ici ? \n");
			perror(exec_list->command->redir[i]);
			exit(1);
		}
	}
	return (fd);
}

int	open_fd_in(int i, t_exec *exec_list)
{
	int	fd;

	fd = open(exec_list->command->redir[i], O_RDONLY);
	printf("-> open fd in for file %s, got fd : %d\n", exec_list->command->redir[i], fd);
	if ( fd == -1)
		return (perror(exec_list->command->redir[i]), -1);
	return (fd);
}

void	open_fds(t_exec *exec_list, int *fd_in, int *fd_out)
{
	printf("in open fds \n");
	int i;
	
	i = 0;
	while (exec_list->command->redir[i])
	{
		printf("-> need to open fd for file %s, kind : %d\n", exec_list->command->redir[i], exec_list->command->redir_kind[i]);
		if (exec_list->command->redir_kind[i] == IN)
		{
			printf("je viens la ? \n");
			*fd_in = open_fd_in(i, exec_list);
			if (*fd_in == -1)
				exit(1);	
		}
		if (exec_list->command->redir_kind[i] == OUT ||
			exec_list->command->redir_kind[i] == OUT_APP)
		{
			printf("-> need to open fd out for file %s\n", exec_list->command->redir[i]);	
			*fd_out = open_fd_out(i, exec_list);
			if (*fd_out == -1)
				exit(1);
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
