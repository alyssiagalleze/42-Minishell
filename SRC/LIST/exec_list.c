/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:52:46 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/17 12:48:20 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_list_init_command(t_command *command)
{
	int	i;

	i = 0;
	while (i < ARG_MAX)
	{
		command->argv[i] = 0;
		command->redir[i] = 0;
		command->redir_kind[i] = 0;
		command->hdoc_fd[i] = -1;
		i ++;
	}
	command->is_var = FALSE;
}

void	exec_list_init_node(t_exec *exec)
{
	exec->pids = NULL;
	exec->is_command = FALSE;
	exec->is_subshell = FALSE;
	exec->command = NULL;
	exec->subshell = NULL;
	exec->next = NULL;
}

t_exec	*exec_list_add_node(t_exec **exec_list_start)
{
	t_exec	*new_node;
	t_exec	*curr_node;

	new_node = malloc(sizeof(t_exec));
	if (new_node == NULL)
		return (NULL);
	if (*exec_list_start != NULL)
	{
		curr_node = *exec_list_start;
		while (curr_node->next)
		{
			curr_node = curr_node->next;
		}
		curr_node->next = new_node;
	}
	else
	{
		*exec_list_start = new_node;
	}
	exec_list_init_node(new_node);
	return (new_node);
}
