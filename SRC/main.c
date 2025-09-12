/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:26:27 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/11 19:41:42 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main()
{
	char	*cmd_line;
	
	cmd_line = NULL;
	while (1)
	{
		cmd_line = readline(PROMPT);
		if (!is_str_empty_or_null(cmd_line))
			add_history(cmd_line);
		else
			break;
	}
	printf("%s", RESET_FONT);
	return (0);
}
