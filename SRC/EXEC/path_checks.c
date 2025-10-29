/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 15:28:28 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/29 13:08:35 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_slash(char *cmd_name)
{
	int	i;

	i = 0;
	while (cmd_name[i])
	{
		if (cmd_name[i] == '/')
			return (TRUE);
		i ++;
	}
	return (FALSE);
}

char	*check_access(char *cmd_path, int *first_errno)
{
	if (!access(cmd_path, F_OK))
	{
		if (access(cmd_path, X_OK) == -1)
		{
			if (*first_errno == 0)
				*first_errno = errno;
			free(cmd_path);
		}
		else
			return (cmd_path);
	}
	else
		free(cmd_path);
	return (NULL);
}
