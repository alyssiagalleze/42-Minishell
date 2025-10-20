/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:43:08 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/20 12:37:57 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int prepare_pipe(t_exec *exec_list, int pipefds[2])
{
	pipefds[0] = -1;
	pipefds[1] = -1;
	if (exec_list->next)
	{
		if (pipe(pipefds) == -1)
			return (perror("pipe"), 1);
	}
	return (0);
}

int	handle_fork_error(int pipefds[2])
{
	if (pipefds[0] != -1)
		close(pipefds[0]);
	if (pipefds[1] != -1)
		close(pipefds[1]);
	perror("fork");
	return (1);
}

void free_env_array(char **envp)
{
	int i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

void	init_std_fds(struct s_data *subshell_data)
{
	subshell_data->std_fds[0] = dup(STDIN_FILENO);
	if (subshell_data->std_fds[0] == -1)
		return (perror("dup"));
	subshell_data->std_fds[1] = dup(STDOUT_FILENO);
	if (subshell_data->std_fds[1] == -1)
		return (perror("dup"));
}
