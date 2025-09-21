/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:27:47 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/21 16:36:52 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parser_check_contr_operator(t_token *token, int prev_type, int prev_kind)
{
	if (prev_type == NONE || prev_type == REDIR_OPERATOR
		|| prev_type == CONTR_OPERATOR || prev_kind == BRACKET_O)
	{
		print_err(PROMPT, "syntax error near unexpected token `",
			token->str, "\'\n");
		return (TRUE);
	}
	if (token->next == NULL)
	{
		print_err(PROMPT, "syntax error: unexpected end of file\n",
			NULL, NULL);
		return (TRUE);
	}
	return (FALSE);
}

static int	parser_check_redir_operator(t_token *token, int prev_type)
{
	if (prev_type == REDIR_OPERATOR)
	{
		print_err(PROMPT, "syntax error near unexpected token `",
			token->str, "\'\n");
		return (TRUE);
	}
	if (token->next == NULL)
	{
		print_err(PROMPT, "syntax error near unexpected token `newline'\n",
			NULL, NULL);
		return (TRUE);
	}
	return (FALSE);
}

static int	parser_check_bracket(
	t_token *token, int prev_type, int prev_kind, int *open_brackets)
{
	if (token->kind == BRACKET_O)
	{
		*open_brackets += 1;
		if (prev_type != NONE && prev_type != CONTR_OPERATOR
			&& prev_kind != BRACKET_O)
		{
			print_err(PROMPT, "syntax error near unexpected token `('\n",
				NULL, NULL);
			return (TRUE);
		}
	}
	else if (token->kind == BRACKET_C)
	{
		*open_brackets -= 1;
		if (*open_brackets < 0 || prev_type == REDIR_OPERATOR
			|| prev_type == CONTR_OPERATOR || prev_kind == BRACKET_O)
		{
			print_err(PROMPT, "syntax error near unexpected token `)'\n",
				NULL, NULL);
			return (TRUE);
		}
	}
	return (FALSE);
}

static int	parser_check_token(
	t_token **token_list, t_token *prev_token, int *open_brackets, int *line_has_cmd)
{
	t_token	*token;
	enum	e_type	prev_type;
	enum	e_kind 	prev_kind;

	token = *token_list;
	if (token->type == NONE)
	{
		print_err(PROMPT, "syntax error : invalid operator ",
			token->str, "\n");
		return (TRUE);
	}
	if (!prev_token)
	{
		prev_type = NONE;
		prev_kind = UNKNOWN;
	}
	else
	{
		prev_type = prev_token->type;
		prev_kind = prev_token->kind;
	}
	if (token->type == REDIR_OPERATOR)
		return (parser_check_redir_operator(token, prev_type));
	if (token->type == WORD)
		return (parser_check_and_assign_word(&token, prev_type, prev_kind, line_has_cmd));
	*line_has_cmd = FALSE;
	if (token->type == CONTR_OPERATOR)
		return (parser_check_contr_operator(token, prev_type, prev_kind));
	if (token->type == BRACKET)
		return (parser_check_bracket(token, prev_type, prev_kind, open_brackets));
	return (FALSE);
}

void	parser(t_token **token_list)
{
	t_token				*token;
	t_token				*prev_token;
	int					brackets;
	int					line_has_cmd;

	token = *token_list;
	prev_token = NULL;
	brackets = 0;
	line_has_cmd = FALSE;
	while (token)
	{
		if (parser_check_token(&token, prev_token, &brackets, &line_has_cmd))
		{
			clean_token_list(token_list);
			return ;
		}
		prev_token = token;
		token = token->next;
	}
	if (brackets)
	{
		print_err(PROMPT, "unexpected EOF while looking for matching `(\'\n",
			PROMPT, "syntax error: unexpected end of file\n");
		clean_token_list(token_list);
	}
}
