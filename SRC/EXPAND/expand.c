/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:52:26 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/16 19:52:53 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO : not just this
int	is_expandable_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (TRUE);
	if (c >= '0' && c <= '9')
		return (TRUE);
	if (c == '_')
		return (TRUE);
	return (FALSE);
}

void	expand_unquote(char *str)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	c = FALSE;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && (c == FALSE || c == str[i]))
		{
			if (c == str[i])
				c = FALSE;
			else if (c == FALSE)
				c = str[i];
			i++;
			continue ;
		}
		if (str[j] != str[i])
			str[j] = str[i];
		i ++;
		j ++;
	}
	while (j < i)
		str[j++] = '\0';
}

void	check_expand_unquote(t_token *token_list)
{
	while (token_list
		&& token_list->type != CONTR_OPERATOR && token_list->type != BRACKET)
	{
		if (!token_list->dollar_expanded)
			expand_unquote(token_list->str);
		token_list = token_list->next;
	}
}

int	expand_command(t_token *token_list, t_env **env)
{
	enum e_err	err;

	err = check_expand_dollar(token_list, *env);
	if (err)
		return (err);
	err = check_expand_asterisk(token_list);
	if (err)
		return (err);
	err = check_expand_wordsplit(token_list);
	if (err)
		return (err);
	check_expand_unquote(token_list);
	return (ERR_SUCCESS);
}
