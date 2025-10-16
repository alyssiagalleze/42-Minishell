/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 16:51:35 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/15 18:11:31 by agalleze         ###   ########.fr       */
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
