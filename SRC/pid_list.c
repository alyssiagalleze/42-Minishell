/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 15:22:39 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/13 17:02:36 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pid_list	*pid_new(pid_t pid)
{
	t_pid_list	*node;

	node = malloc(sizeof(t_pid_list));
	if (!node)
		return (NULL);
	node->pid = pid;
	node->next = NULL;
	return (node);
}

void	pid_add_back(t_pid_list **lst, pid_t pid)
{
	t_pid_list	*new;
	t_pid_list	*tmp;

	new = malloc(sizeof(t_pid_list));
	if (!new)
		return ;
	new->pid = pid;
	new->next = NULL;
	if (!*lst)
		*lst = new;
	else
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	clean_pid(t_pid_list **list)
{
	t_pid_list	*tmp;

	while (*list)
	{
		tmp = (*list)->next;
		free(*list);
		*list = tmp;
	}
}

// int	pid_wait_all(t_pid_list *list, int status)
// {
// 	int	exit_status;

// 	exit_status = 0;
// 	while (list)
// 	{
// 		waitpid(list->pid, &status, 0);
// 		list = list->next;
// 	}
// 	if (WIFEXITED(status))
// 		exit_status = WEXITSTATUS(status);
// 	else
// 		exit_status = status;
// 	return (exit_status);
// }
