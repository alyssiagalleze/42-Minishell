/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 12:42:42 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_loop(struct s_data *data, int *status)
{
	char	*input;
	int		cmd_count;

	*status = 0;
	cmd_count = 0;
	while (1)
	{
		init_readline_signals();
		prev_command_check_signal(status);
		g_signal = 0;
		save_last_status(status, &data->env);
		data_reset_pointers(data);
		get_input(&input, data, status, &cmd_count);
		lexer(&data->token_list, input, data);
		if (is_longer_than_arg_max(data, status))
			continue ;
		else if (heredocs(data->token_list, cmd_count, data->env))
			my_exit(2, &data->env, &input, &data->token_list_head);
		if (heredoc_check_signal(data, status))
			continue ;
		if (parser(&data->token_list, &data->token_list_head, status))
			*status = token_list_to_exec(data);
	}
}

int	g_signal = 0;

int	main(int ac, char **av, char **env)
{
	struct s_data	data;
	int				status;

	(void)ac;
	(void)av;
	check_std_errors();
	g_signal = 0;
	if (init_env_list(env, &data.env))
		return (print_err(PROMPT, E_MALLOC, NULL, NULL), 2);
	status = 0;
	shell_loop(&data, &status);
	exit_clean(&data);
	return (status);
}

//echo *"a"*