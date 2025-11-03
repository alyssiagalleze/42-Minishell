/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:12:31 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 12:29:25 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_expand_dollar(t_token *token)
{
	int		i;
	int		j;

	i = 0;
	while (token->str[i])
	{
		if (token->str[i] == '\'')
		{
			i ++;
			j = i;
			while (token->str[i] && token->str[i] != '\'')
				i ++;
			if (!token->str[i])
			{
				i = j;
				continue ;
			}
		}
		else if (str_ncmp(token->str + i, "$?", 2, FALSE)
			|| (token->str[i] == '$' && is_expandable_char(token->str[i + 1])))
			return (i);
		i ++;
	}
	return (-1);
}

void	dollar_expand_unquote(char *str)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (str[i] == '"')
			{
				i ++;
				continue ;
			}
		}
		if (str[j] != str[i])
			str[j] = str[i];
		i ++;
		j ++;
	}
	while (j < i)
		str[j++] = '\0';
}

int	check_expand_dollar(t_token *token_list, t_env *env)
{
	int			index;
	enum e_err	err;

	while (token_list && token_list->type != CONTR_OPERATOR
		&& token_list->type != BRACKET)
	{
		index = -1;
		if (token_list->type == WORD)
			index = should_expand_dollar(token_list);
		if (index == -1)
			token_list = token_list->next;
		else
		{
			if (!token_list->dollar_expanded)
			{
				dollar_expand_unquote(token_list->str);
				token_list->dollar_expanded = TRUE;
				continue ;
			}
			err = expand_dollar(token_list, env, index);
			if (err)
				return (err);
		}
	}
	return (ERR_SUCCESS);
}
