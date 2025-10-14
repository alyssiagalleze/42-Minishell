/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:53:12 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/13 19:13:27 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_exit(int status, t_env **my_env, char **input, t_token **token_list_head)
{
	cleaner(my_env, input, token_list_head);
	// write(1, "exit\n", 5);
	printf("%s", RESET_FONT);
	exit(status);
}
