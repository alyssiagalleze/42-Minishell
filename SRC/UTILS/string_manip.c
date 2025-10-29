/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manip.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:41:32 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/28 17:45:28 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_white_space(const char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int	is_char_operator(const char c)
{
	return (is_char_in_string(c, METACHARACTERS, FALSE, FALSE));
}

int	is_char_separator(const char c)
{
	return (is_char_in_string(c, METASEPARATORS, FALSE, FALSE));
}

int	is_char_in_string(
	const char c, const char *str, int accept_null, int give_index)
{
	int	i;

	i = 0;
	if (!accept_null && !str)
	{
		write(2, "err : sending a null string in is_char_in_string !\n", 52);
		if (give_index)
			return (-1);
		return (FALSE);
	}
	while (str[i])
	{
		if (c == str[i])
		{
			if (give_index)
				return (i);
			return (TRUE);
		}
		i ++;
	}
	return (FALSE);
}

int	is_str_empty_or_null(const char *str)
{
	while (str && *str)
	{
		if (!is_white_space(*str))
			return (FALSE);
		str ++;
	}
	return (TRUE);
}
