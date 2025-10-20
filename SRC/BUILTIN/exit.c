/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:53:12 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/20 17:48:30 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_exit(int status, t_env **my_env, char **input, t_token **token_list_head)
{
	cleaner(my_env, input, token_list_head);
	// write(1, "exit\n", 5);
	printf("%s", RESET_FONT);
	printf("exit\n");
	exit(status);
}

// int is_only_digit(char *arg)
// {
// 	int	i;

// 	i = 0;
// 	while (arg[i])
// 	{
// 		if (!(arg[i] >= '0' && arg[i] <= '9'))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// void	my_exit(char **args, t_env **env, char **input, t_token **token_list_head)
// {
// 	int	status;

// 	status = 0;
// 	if (is_only_digit(args[1]))
// 	{
// 		print_err(PROMPT, ": exit: ", args[1], ": numeric argument required");
// 		exit(2);
// 	}
// 	if (args[2] != NULL)
// 	{
// 		print_err(PROMPT, ": exit: too many arguments", NULL, NULL);
// 		exit(2);
// 	}
// 	status = ft_atoi(args[1]);
// 	cleaner(env, input, token_list_head);
// 	printf("%s", RESET_FONT);
// 	exit(status);
// }
