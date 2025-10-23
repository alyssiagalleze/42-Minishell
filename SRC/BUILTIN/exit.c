/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:53:12 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/23 15:08:57 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_exit(
	int status, t_env **my_env, char **input, t_token **token_list_head)
{
	cleaner(my_env, input, token_list_head);
	printf("%s", RESET_FONT);
	printf("exit\n");
	exit(status);
}

void	my_exit_builtin(
	t_exec *exec_list, struct s_exec_data *exec_data, char **input)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	printf("args[0] = %s\n", exec_list->command->argv[0]);
	if (exec_list->command->argv[1] && !only_digit(exec_list->command->argv[1]))
	{
		print_err(PROMPT, ": exit: ",
			exec_list->command->argv[1], ": numeric argument required");
		exit(2);
	}
	if (exec_list->command->argv[2] != NULL)
		(print_err(PROMPT, ": exit: too many arguments", NULL, NULL), exit(2));
	if (exec_list->command->argv[1])
		status = ft_atoi(exec_list->command->argv[1]);
	if (!exec_data->is_pipe)
		printf("%s", RESET_FONT);
	if (input && !exec_data->is_pipe)
		clean_input(input);
	clean_data_close_fds(exec_data, exec_list);
	exit(status);
}
