/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:01:11 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 14:40:52 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*exec_cleaner(char **path_tab, char *path, char *cmd_path)
{
	int	i;

	i = 0;
	if (path_tab)
	{
		while (path_tab[i])
			free(path_tab[i++]);
		free(path_tab);
	}
	if (path)
	{
		free(path);
	}
	return (cmd_path);
}

void	clean_exec_command_node(t_exec *exec_list)
{
	int	i;

	i = 0;
	while (exec_list->command->argv[i])
	{
		free(exec_list->command->argv[i]);
		exec_list->command->argv[i] = NULL;
		i ++;
	}
	free(exec_list->command);
	exec_list->command = NULL;
}

void	clean_exec_subshell_node(t_exec *exec_list)
{
	if (exec_list->subshell->token_sublist)
		clean_token_list(&exec_list->subshell->token_sublist, FALSE);
	free(exec_list->subshell);
}

void	clean_exec_list(t_exec **exec_list)
{
	while (exec_list && *exec_list)
		clean_exec_node(exec_list);
}

void	clean_exec_node(t_exec **exec_list)
{
	t_exec	*next;

	next = (*exec_list)->next;
	if (exec_list && *exec_list)
	{
		if ((*exec_list)->is_command && (*exec_list)->command)
			clean_exec_command_node(*exec_list);
		else if ((*exec_list)->is_subshell && (*exec_list)->subshell)
			clean_exec_subshell_node(*exec_list);
		free(*exec_list);
	}
	*exec_list = next;
}
