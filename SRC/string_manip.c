/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manip.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:41:32 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/14 19:11:35 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_char_white_space(const char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int		is_char_operator(const char c)
{
	return (is_char_in_string(c, METACHARACTERS, FALSE));
}

int		is_char_separator(const char c)
{
	return (is_char_in_string(c, METASEPARATORS, FALSE));
}

int		is_char_in_string(const char c, const char *str, int accept_null)
{
	int	i;

	i = 0;
	if (!accept_null && !str)
	{
		write(2, "err : sending a null string in is_char_in_string !\n", 52);
		return (FALSE);
	}
	while (str[i])
	{
		if (c == str[i])
		{
			return (TRUE);
		}
		i ++;
	}
	return (FALSE);
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

int	str_cmp(char *str1, char *str2, int accept_null)
{
	if (!accept_null && (str1 == NULL || str2 == NULL))
	{
		write(2, "err : str_cmp received a NULL string !\n", 40);
		return (FALSE);
	}
	if (str1 == str2)
	{
		return (TRUE);
	}
	if (str1 == NULL || str2 == NULL)
	{
		return (FALSE);
	}
	while (*str1 && (*str1 == *str2))
	{
		str1 ++;
		str2 ++;
	}
	return (*str1 == *str2);
}

char	*extract_string(const char *start, int len)
{
	int		i;
	char	*new_str;
	
	if (start <= 0 || start == NULL )
		return (NULL);
	new_str = malloc(sizeof(char) * len);
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = start[i];
		i ++;
	}
	new_str[i] = 0;
	return (new_str);
}
