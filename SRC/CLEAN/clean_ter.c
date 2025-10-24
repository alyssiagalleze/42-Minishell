/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_ter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:51:09 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/24 17:01:37 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_data_close_fds(
	struct s_exec_data *exec_data, t_exec *exec_list, int is_in_child)
{
	if (exec_data->env && is_in_child)
		clean_env(&exec_data->env);
	if (is_in_child && exec_data->token_list)
		clean_token_list(&exec_data->token_list, TRUE);
	if (exec_data->prev_fd != -1)
		close(exec_data->prev_fd);
	if (is_in_child)
		close_fds(exec_data->pipefds, exec_data->saved_stds);
	if (exec_list)
		clean_exec_list(&exec_list);
}
