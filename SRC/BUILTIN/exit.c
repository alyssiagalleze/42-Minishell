/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:53:12 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/21 18:00:17 by agalleze         ###   ########.fr       */
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

char	**split_into_words(char *input)
{
	char **args;

	args = ft_split(input, ' ');
	if (!args)
		return (print_err(PROMPT, PERR_MALLOC, NULL, NULL), NULL);
	return (args);
}

void	my_exit_builtin(char **args, t_env **env, char **input, t_token **token_list_head)
{
	int	status;
	int i;
	
	status = 0;
	i = 0;
	printf("args[0] = %s\n", args[0]);
	if (!is_only_digit(args[1]))
	{
		print_err(PROMPT, ": exit: ", args[1], ": numeric argument required");
		exit(2);
	}
	if (args[2] != NULL)
	{
		print_err(PROMPT, ": exit: too many arguments", NULL, NULL);
		exit(2);
	}
	status = ft_atoi(args[1]);
	while (args[i])
		free(args[i++]);
	free(args);
	cleaner(env, input, token_list_head);
	printf("%s", RESET_FONT);
	exit(status);
}
