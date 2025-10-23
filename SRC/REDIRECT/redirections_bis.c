/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:17:34 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/23 18:47:38 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_out_redirection(t_exec *exec_list, int i)
{
	if (exec_list->command->redir_kind[i] == OUT
		|| exec_list->command->redir_kind[i] == OUT_APP)
		return (TRUE);
	else
		return (FALSE);
}

int	is_in_redirection(t_exec *exec_list, int i)
{
	if (exec_list->command->redir_kind[i] == IN
		|| exec_list->command->redir_kind[i] == HDOC)
		return (TRUE);
	else
		return (FALSE);
}

void	restore_std_fds(int saved_stds[2])
{
	if (dup2(saved_stds[0], STDIN_FILENO) == -1)
		perror("dup2 restore stdin");
	if (dup2(saved_stds[1], STDOUT_FILENO) == -1)
		perror("dup2 restore stdout");
	close(saved_stds[0]);
	close(saved_stds[1]);
}
