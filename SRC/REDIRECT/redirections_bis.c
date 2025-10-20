/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:17:34 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/20 15:28:04 by agalleze         ###   ########.fr       */
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
