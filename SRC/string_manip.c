/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manip.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 19:41:32 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/11 19:41:38 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_white_space(const char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int	is_char_operator(const char c)
{
	return (is_char_in_string(c, METACHARACTERS, FALSE));
}

int	is_char_separator(const char c)
{
	return (is_char_in_string(c, METASEPARATORS, FALSE));
}

int	is_char_in_string(const char c, const char *str, int accept_null)
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

int	str_ncmp(char *str1, char *str2, int n, int accept_null)
{
	int	i;

	if (!accept_null && (str1 == NULL || str2 == NULL))
	{
		write(2, "err : str_ncmp received a NULL string !\n", 41);
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
	i = 0;
	while (*str1 && (*str1 == *str2) && i < n)
	{
		i ++;
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
	new_str = malloc(sizeof(char) * (len + 1));
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

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	int			i;
	size_t		len;
	char		*ns;

	i = 0;
	len = ft_strlen(s);
	if (!s)
	{
		ns = malloc(1);
		if (ns)
			ns[0] = '\0';
		return (ns);
	}
	ns = malloc((len + 1) * sizeof(char));
	if (!ns)
		return (NULL);
	while (s[i])
	{
		ns[i] = s[i];
		i++;
	}
	ns[i] = '\0';
	return (ns);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s1 || !s2) //TODO : FALSE, and malloc can break
		s1 = ft_strdup("");
	len = ft_strlen(s1) + ft_strlen(s2);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

// TODOLONG
char	*str_append_sq(char *from, char *app)
{
	char	*new_str;
	int		i;
	int		j;

	if (app == NULL)
		return (from);
	new_str = malloc(sizeof(char) * (ft_strlen(from) + ft_strlen(app) + 4));
	if (new_str == NULL)
		return (free(from), free(app), NULL);
	i = 0;
	while (from && from[i])
	{
		new_str[i] = from[i];
		i ++;
	}
	if (from)
		new_str[i++] = ' ';
	new_str[i++] = '\'';
	j = 0;
	while (app[j])
	{
		new_str[i + j] = app[j];
		j ++;
	}
	new_str[i + j++] = '\'';
	new_str[i + j++] = '\0';
	free(from);
	return (new_str);
}
