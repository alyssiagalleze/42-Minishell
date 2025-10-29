/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exec_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:20:07 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/28 16:24:43 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_and_or(t_token *token_list)
{
	if (!token_list)
		return (FALSE);
	if (token_list->kind == AND || token_list->kind == OR)
		return (TRUE);
	return (FALSE);
}

int	should_exec_pipeline(t_token *token, int lvalue)
{
	if (token->kind == AND && lvalue)
		return (FALSE);
	if (token->kind == OR && !lvalue)
		return (FALSE);
	return (TRUE);
}

void	skip_subshell_tokens(t_token **token_list)
{
	int	open_bracket;

	open_bracket = 1;
	(*token_list) = (*token_list)->next;
	while (open_bracket)
	{
		if ((*token_list)->kind == BRACKET_O)
			open_bracket ++;
		if ((*token_list)->kind == BRACKET_C)
			open_bracket --;
		*token_list = (*token_list)->next;
	}
}

void	skip_command_tokens(t_token **token_list)
{
	while (*token_list)
	{
		if ((*token_list)->type == WORD
			|| (*token_list)->type == REDIR_OPERATOR)
		(*token_list) = (*token_list)->next;
		else
			return ;
	}
}

void	skip_pipeline_tokens(t_token **token_list)
{
	if (is_and_or(*token_list))
		*token_list = (*token_list)->next;
	while (*token_list && (*token_list)->type != CONTR_OPERATOR)
	{
		if ((*token_list)->kind == BRACKET_O)
			skip_subshell_tokens(token_list);
		else
			skip_command_tokens(token_list);
		if (*token_list && (*token_list)->kind == PIPE)
			*token_list = (*token_list)->next;
	}
}
