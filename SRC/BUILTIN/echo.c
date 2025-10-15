/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:29:30 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/13 13:17:25 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_n_enabled(char *arg)
{
	if (!arg)
		return (FALSE);
	if (*arg == '-')
	{ 
		arg++;
		while (*arg)
		{
			if (*arg != 'n')
				return (FALSE);
			arg++;
		}
		return (TRUE);
	}
	else
		return (FALSE);
}

int  echo(char **args)
{
	int i;
	int	count;
	int	newline;

	count = 0;
	while (args[count])
		count++;
	i = 1;
	newline = TRUE;
	if (is_n_enabled(args[i]))
	{
		i++;
		newline = FALSE;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i++], 1);
		if (i != count)
			write(STDOUT_FILENO, " ", 1);			
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
