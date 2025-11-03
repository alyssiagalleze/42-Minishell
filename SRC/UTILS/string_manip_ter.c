/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manip_ter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:27:08 by agalleze          #+#    #+#             */
/*   Updated: 2025/11/03 14:22:10 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_null_input(char *s1, char *s2, char **str)
{
	if (!s1 && !s2)
	{
		*str = NULL;
		return (0);
	}
	if (!s1)
	{
		*str = ft_strdup(s2);
		if (!(*str))
			*str = NULL;
		return (0);
	}
	else if (!s2)
	{
		*str = ft_strdup(s1);
		if (!(*str))
			*str = NULL;
		return (0);
	}
	return (1);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len;
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if ((!s1 || !s2) && !handle_null_input(s1, s2, &str))
		return (str);
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

char	*str_append(char *from, char *app)
{
	char	*new_str;
	int		i;
	int		j;

	if (app == NULL)
		return (from);
	new_str = malloc(sizeof(*new_str) * (ft_strlen(from) + ft_strlen(app) + 1));
	if (new_str == NULL)
		return (free(from), free(app), NULL);
	i = 0;
	while (from && from[i])
	{
		new_str[i] = from[i];
		i ++;
	}
	j = 0;
	while (app[j])
	{
		new_str[i + j] = app[j];
		j++;
	}
	new_str[i + j] = '\0';
	from = ft_free(from);
	app = ft_free(app);
	return (new_str);
}

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
	j = -1;
	while (app[++j])
		new_str[i + j] = app[j];
	new_str[i + j++] = '\'';
	new_str[i + j++] = '\0';
	free(from);
	return (new_str);
}

char	*ft_strchr(const char *s, int c)
{
	unsigned char	k;

	k = (unsigned char)c;
	while (*s)
	{
		if (k == *s)
			return ((char *)s);
		s++;
	}
	if (k == 0)
		return ((char *)s);
	return (NULL);
}
