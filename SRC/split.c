/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:40:04 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/24 17:31:20 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	ft_count_words(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
		{
			if (s[i] && s[i + 1] == c)
			{
				count++;
				i++;
			}
			i++;
		}
		if (s[i] && s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

char	*ft_strcpy(char *line, char const *s, int a, int b)
{
	int	i;
	int	len;

	i = 0;
	len = b - a;
	while (i < len)
	{
		line[i] = s[a];
		i++;
		a++;
	}
	line[i] = '\0';
	return (line);
}

void	ft_free_split(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**ft_add_words(char const *str, char c, char **tab)
{
	int	i;
	int	word_count;
	int	j;
	int	word_len;

	j = 0;
	i = 0;
	word_count = ft_count_words(str, c);
	while (j < word_count)
	{
		word_len = 0;
		if (str[i] == c)
			i++;
		while (str[i] != c && str[i])
		{
			i++;
			word_len++;
		}
		tab[j] = calloc((word_len + 1), sizeof(char));
		if (!tab[j])
			return (ft_free_split(tab), NULL);
		ft_strcpy(tab[j], str, i - word_len, i);
		j++;
	}
	return (tab[word_count] = NULL, tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		wordcount;

	wordcount = ft_count_words(s, c);
	tab = malloc((wordcount + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	tab = ft_add_words(s, c, tab);
	return (tab);
}
