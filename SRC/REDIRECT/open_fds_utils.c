/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fds_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:57:03 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 14:29:02 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_in_child(t_exec *exec_list, struct s_exec_data *exec_data)
{
	return (exec_data->is_pipe
		|| (!exec_data->is_pipe && !is_builtin(exec_list)));
}

int	handle_open_error(t_exec *exec_list, struct s_exec_data *exec_data, int i)
{
	print_err(PROMPT, NULL, NULL, NULL);
	perror(exec_list->command->redir[i]);
	clean_data_close_fds(exec_data, NULL, is_in_child(exec_list, exec_data));
	if (!exec_data->is_pipe && is_builtin(exec_list))
		return (-1);
	if (exec_data->is_pipe && !exec_list->command->argv[0])
	{
		clean_exec_list(&exec_list);
		exit(0);
	}
	clean_exec_list(&exec_list);
	exit(1);
	return (0);
}

int	open_fd_out(int i, t_exec *exec_list, struct s_exec_data *exec_data)
{
	int	fd;

	if (exec_list->command->redir_kind[i] == OUT_APP)
	{
		fd = open(exec_list->command->redir[i], O_RDWR
				| O_APPEND | O_CREAT, 0777);
		if (fd == -1)
			return (handle_open_error(exec_list, exec_data, i), -1);
	}
	else
	{
		fd = open(exec_list->command->redir[i], O_RDWR
				| O_TRUNC | O_CREAT, 0777);
		if (fd == -1)
			return (handle_open_error(exec_list, exec_data, i), -1);
	}
	return (fd);
}

int	open_hdoc_fds(t_exec *exec_list, int *hdoc_index)
{
	int	fd;

	fd = exec_list->command->hdoc_fd[*hdoc_index];
	*hdoc_index += 1;
	return (fd);
}
