/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:29:04 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/14 19:29:27 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_lexer_print(t_lexer *lexer_node)
{
	printf("\n\n--DEBUG LEXER PRINT--\n");
	while (lexer_node)
	{
		printf("%s\n", lexer_node->str);
		lexer_node = lexer_node->next;
	}
	printf("\n");
}
