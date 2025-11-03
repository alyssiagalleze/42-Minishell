/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:53:12 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 14:40:46 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_exit(
	int status, t_env **my_env, char **input, t_token **token_list_head)
{
	cleaner(my_env, input, token_list_head);
	ft_putstr_fd(RESET_FONT, 1);
	ft_putstr_fd("exit\n", 1);
	exit(status);
}

int	exit_on_error(t_exec *exec_list, struct s_exec_data *exec_data)
{
	if (exec_list->command->argv[1]
		&& !only_digit_no_overflow(exec_list->command->argv[1]))
	{
		print_err(PROMPT, "exit: ",
			exec_list->command->argv[1], ": numeric argument required\n");
		clean_data_close_fds(exec_data, exec_list, 1);
		ft_putstr_fd(RESET_FONT, 1);
		exit(2);
	}
	if (exec_list->command->argv[2] != NULL)
	{
		print_err(PROMPT, "exit: too many arguments\n", NULL, NULL);
		return (1);
	}
	return (0);
}

int	my_exit_builtin(
	t_exec *exec_list, struct s_exec_data *exec_data, char **input)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	if (exit_on_error(exec_list, exec_data))
		return (1);
	if (exec_list->command->argv[1])
		status = ft_atoi(exec_list->command->argv[1]);
	if (!exec_data->is_pipe)
		ft_putstr_fd(RESET_FONT, 1);
	if (input && !exec_data->is_pipe)
		clean_input(input);
	clean_data_close_fds(exec_data, exec_list, 1);
	exit(status);
	return (0);
}
