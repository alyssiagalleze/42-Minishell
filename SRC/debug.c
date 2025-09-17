/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:29:04 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/17 15:41:57 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_lexer_print(t_lexer *lexer_node)
{
	printf("\n\n--DEBUG LEXER PRINT--\n");
	while (lexer_node)
	{
		printf("%s  // ", lexer_node->str);
		if (lexer_node->token_type == NONE)
			printf("none\n");
		if (lexer_node->token_type == WORD)
			printf("word\n");
		if (lexer_node->token_type == CONTR_OPERATOR)
			printf("control operator\n");
		if (lexer_node->token_type == REDIR_OPERATOR)
			printf("redirection operator\n");
		if (lexer_node->token_type == BRACKET)
			printf("bracket\n");
		printf("\n");
		lexer_node = lexer_node->next;
	}
	printf("\n");
}
