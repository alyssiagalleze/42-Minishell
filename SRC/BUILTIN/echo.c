/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:29:30 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/06 15:56:20 by tfiette          ###   ########.fr       */
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
	
	count = 0;
	while (args[count])
		count++;
	i = 1;
	if (is_n_enabled(args[i]))
	{
		i++;
		while (args[i])
		{
			ft_putstr_fd(args[i++], 1);
			if (i != count)
				write(1, " ", 1);
		}
		return (0);
	}
	while (args[i])
	{
		ft_putstr_fd(args[i++], 1);
		if (i != count)
			write(1, " ", 1);			
	}
	return (write(1, "\n", 1), 0);
}
