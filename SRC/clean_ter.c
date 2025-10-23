/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bis_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:51:09 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/23 17:53:21 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_data_close_fds(
	struct s_exec_data *exec_data, t_exec *exec_list, int is_in_child)
{
	if (exec_data->env)
		clean_env(&exec_data->env);
	if (is_in_child && exec_data->token_list)
	{
		clean_token_list(&exec_data->token_list);
	}
	if (exec_data->saved_stds[0] != -1 || exec_data->saved_stds[1] != -1)
		restore_std_fds(exec_data->saved_stds);
	if (exec_data->prev_fd != -1)
		close(exec_data->prev_fd);
	close_fds(exec_data->pipefds, exec_data->saved_stds);
	if (exec_list)
		clean_exec_list(&exec_list);
}
