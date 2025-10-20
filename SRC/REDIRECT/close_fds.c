/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 12:33:40 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/20 12:35:46 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_and_exit(int pipefds_r, int pipefds_w, char **my_env, int status)
{
	if (pipefds_r != -1)
		close(pipefds_r);
	if (pipefds_w != -1)
		close(pipefds_w);
	if (my_env)
		free_env_array(my_env);
	exit(status);
}

int	close_no_exit(int pipefds_r, int pipefds_w, int status)
{
	if (pipefds_r != -1)
		close(pipefds_r);
	if (pipefds_w != -1)
		close(pipefds_w);
	return (status);
}

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
		if (saved_stds[0] != -1)
			close(saved_stds[0]);
		if (saved_stds[1] != -1)
			close(saved_stds[1]);
	}
}
