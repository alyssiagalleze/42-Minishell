/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:47:41 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 10:51:48 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parser_check_last_word(t_token *token)
{
	int	i;
	int	single_quotes;
	int	double_quotes;

	i = 0;
	single_quotes = FALSE;
	double_quotes = FALSE;
	if (token->next != NULL)
		return (FALSE);
	while (token->str[i])
	{
		if (token->str[i] == SINGLE_QUOTE && !double_quotes)
			single_quotes = (single_quotes == FALSE);
		if (token->str[i] == DOUBLE_QUOTE && !single_quotes)
			double_quotes = (double_quotes == FALSE);
		i ++;
	}
	if (single_quotes)
		print_err(PROMPT, E_QUOTE, PROMPT, E_STX_EOF);
	if (double_quotes)
		print_err(PROMPT, E_QUOTES, PROMPT, E_STX_EOF);
	return (single_quotes || double_quotes);
}

int	parser_check_and_assign_word(
			t_token **token, int prev_type, int prev_kind, int *line_has_cmd)
{
	if (parser_check_last_word(*token))
		return (TRUE);
	if (prev_type == REDIR_OPERATOR)
	{
		(*token)->kind = WORD_FILE;
	}
	else if ((prev_kind != WORD_COM && prev_kind != WORD_ARG)
		&& is_char_in_string('=', (*token)->str, FALSE, FALSE))
	{
		(*token)->kind = WORD_VAR;
	}
	else if (!*line_has_cmd)
	{
		(*token)->kind = WORD_COM;
		*line_has_cmd = TRUE;
	}
	else
	{
		(*token)->kind = WORD_ARG;
	}
	return (FALSE);
}

int	parser_clean_failure(t_token **token_list_head, int *status, int error_id)
{
	if (error_id == 1)
		print_err(PROMPT, E_STX_NL, NULL, NULL);
	else if (error_id == 2)
		print_err(PROMPT, E_BRA_O, PROMPT, E_STX_EOF);
	*status = 2;
	cleaner(NULL, NULL, token_list_head);
	return (FALSE);
}
