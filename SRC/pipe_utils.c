/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:43:08 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/13 11:50:51 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int prepare_env_and_pipe(t_exec *exec_list, t_env **env, char ***my_env, int pipefds[2])
{
	pipefds[0] = -1;
	pipefds[1] = -1;
	if (!exec_list || !env)
		return (print_err(PROMPT, ": internal: prepare_env_and_pipe bad args\n", NULL, NULL), 1);
	*my_env = transfer_env(env);
	if (!*my_env)
		return (print_err(PROMPT, ": malloc: ", "environment transfer failed.", NULL), 2);
	if (exec_list->next)
	{
		// printf("Check fds for command %s\n", exec_list->command->argv[0]);
		if (pipe(pipefds) == -1)
			return (perror("pipe"), 1);
	}
	// printf("pipefd[0] = %d, pipefd[1] = %d\n", pipefds[0], pipefds[1]);
	return (0);
}

int	handle_fork_error(int pipefds[2], char **my_env)
{
	if (pipefds[0] != -1)
		close(pipefds[0]);
	if (pipefds[1] != -1)
		close(pipefds[1]);
	if (my_env)
		free_env_array(my_env);
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
