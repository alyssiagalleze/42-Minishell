/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:53:12 by agalleze          #+#    #+#             */
/*   Updated: 2025/09/30 15:40:51 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_exit(int status, t_env **my_env, char **input, t_token **token_list_save)
{
	cleaner(my_env, input, token_list_save);
	write(1, "exit\n", 5);
	printf("%s", RESET_FONT);
	exit(status);
}
