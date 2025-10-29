/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:12:58 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/28 16:24:51 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match_an_operator_pattern(char *str, int length)
{
	if (str_ncmp(str, _OR, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _AND, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _PIPE, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _OUT, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _IN, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _HDOC, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _OUT_APP, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _BRACKET_O, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _BRACKET_C, length, FALSE))
		return (TRUE);
	return (FALSE);
}

int	get_operator_type(char *str)
{
	if (str_cmp(str, _OR, TRUE)
		|| str_cmp(str, _AND, TRUE)
		|| str_cmp(str, _PIPE, TRUE))
		return (CONTR_OPERATOR);
	if (str_cmp(str, _OUT, TRUE)
		|| str_cmp(str, _IN, TRUE)
		|| str_cmp(str, _HDOC, TRUE)
		|| str_cmp(str, _OUT_APP, TRUE))
		return (REDIR_OPERATOR);
	if (str_cmp(str, _BRACKET_O, TRUE)
		|| str_cmp(str, _BRACKET_C, TRUE))
		return (BRACKET);
	return (NONE);
}

int	get_operator_kind(char *str)
{
	if (str_cmp(str, _OR, TRUE))
		return (OR);
	if (str_cmp(str, _AND, TRUE))
		return (AND);
	if (str_cmp(str, _PIPE, TRUE))
		return (PIPE);
	if (str_cmp(str, _OUT, TRUE))
		return (OUT);
	if (str_cmp(str, _IN, TRUE))
		return (IN);
	if (str_cmp(str, _HDOC, TRUE))
		return (HDOC);
	if (str_cmp(str, _OUT_APP, TRUE))
		return (OUT_APP);
	if (str_cmp(str, _BRACKET_O, TRUE))
		return (BRACKET_O);
	if (str_cmp(str, _BRACKET_C, TRUE))
		return (BRACKET_C);
	return (UNKNOWN);
}
