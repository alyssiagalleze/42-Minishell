/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manip.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:41:32 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/11 19:42:00 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_char_white_space(const char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int is_str_empty_or_null(const char *str)
{
	while (str && *str)
	{
		if (!is_char_white_space(*str))
			return (FALSE);
		str ++;
	}
	return (TRUE);
}
