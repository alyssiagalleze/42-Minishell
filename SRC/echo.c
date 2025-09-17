/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:29:30 by agalleze          #+#    #+#             */
/*   Updated: 2025/09/16 18:10:05 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_n_enabled(char *arg)
{
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
	i = 0;
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
