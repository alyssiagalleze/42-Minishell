/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk_bis.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:03:41 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/11 19:40:05 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hide_file(char *file, char *pattern)
{
	while (*pattern == '*')
		pattern++;
	if (*file == '.' && *pattern != '.')
		return (TRUE);
	return (FALSE);
}

int	is_dir_only(char **pattern)
{
	char	*ptr;

	ptr = *pattern;
	while (*ptr)
		ptr++;
	ptr--;
	if (ptr >= *pattern)
	{
		if (ptr >= *pattern && *ptr == '/')
		{
			while (ptr >= *pattern && *ptr == '/')
			{
				*ptr = '\0';
				ptr --;
			}
			return (TRUE);
		}
	}
	return (FALSE);
}

int	is_dir(struct dirent *file_info)
{
	if (file_info->d_type == 0 || file_info->d_type == 4)
		return (TRUE);
	return (FALSE);
}

int	match_asterisk_pattern_bt(char *str, char *pattern)
{
	if (!*pattern && !*str)
		return (TRUE);
	if (*pattern != '*')
	{
		if (*pattern == *str)
			return (match_asterisk_pattern_bt(str + 1, pattern + 1));
		else
			return (FALSE);
	}
	while (*pattern == '*')
	{
		pattern ++;
		if (!*pattern)
			return (TRUE);
	}
	while (*str)
	{
		if (match_asterisk_pattern_bt(str++, pattern))
			return (TRUE);
	}
	return (FALSE);
}

int	should_expand_file(
	char *file_name, char *pattern, int dir_only, struct dirent *file)
{
	if (match_asterisk_pattern_bt(file_name, pattern))
	{
		if (!hide_file(file_name, pattern)
			&& !(dir_only == TRUE && !is_dir(file)))
			return (TRUE);
	}
	return (FALSE);
}
