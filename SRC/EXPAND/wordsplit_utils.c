/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordsplit_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:30:18 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/17 19:37:11 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ws_count_words(char *str)
{
	int		i;
	char	c;
	int		word_count;

	word_count = 0;
	i = 0;
	while (str[i])
	{
		word_count ++;
		while (str[i] && !is_white_space(str[i]))
		{
			if (str[i] == '\'' || str [i] == '"')
			{
				c = str[i];
				i++;
				while (str[i] && str[i] != c)
					i ++;
			}
			i ++;
		}
		while (is_white_space(str[i]))
			i ++;
	}
	return (word_count);
}

int	ws_allocate_string(char **tab, char *str, int curr_char, int curr_word)
{
	int		i;
	char	*str_ptr;
	char	c;

	i = 0;
	str_ptr = str + curr_char;
	while (is_white_space(*str_ptr))
		str_ptr++;
	while (str_ptr[i] && !is_white_space(str_ptr[i]))
	{
		if (str_ptr[i] == '\'' || str_ptr[i] == '"')
		{
			c = str_ptr[i];
			i++;
			while (str_ptr[i] && str_ptr[i] != c)
				i ++;
		}
		i ++;
	}
	tab[curr_word] = malloc(sizeof(**tab) * (i + 1));
	if (tab[curr_word] == NULL)
		return (-1);
	return (str_ptr - str + i);
}

int	ws_fill_string(char *to, char *from, int curr_char)
{
	int		i;
	char	*from_ptr;
	char	c;

	i = 0;
	from_ptr = from + curr_char;
	while (is_white_space(*from_ptr))
		from_ptr++;
	while (from_ptr[i] && !is_white_space(from_ptr[i]))
	{
		if (from_ptr[i] == '\'' || from_ptr[i] == '"')
		{
			to[i] = from_ptr[i];
			c = from_ptr[i++];
			while (from_ptr[i] && from_ptr[i] != c)
			{
				to[i] = from_ptr[i];
				i++;
			}
		}
		to[i] = from_ptr[i];
		i++;
	}
	to[i] = '\0';
	return (from_ptr - from + i);
}

void	check_wordsplit_failure(char **tab, const int word_count)
{
	int	words;
	int	failure;

	words = 0;
	failure = FALSE;
	if (tab == NULL)
		return ;
	while (words < word_count)
	{
		if (tab[words] == NULL)
			failure = TRUE;
		words++;
	}
	if (failure == TRUE)
	{
		// printf("Found failure\n");
		while (words)
		{
			words--;
			if (tab[words])
				free(tab[words]);
		}
		free(tab);
	}
}

void	ws_free_tab(char **tab, int word_count)
{
	int	i;

	i = 0;
	while (i < word_count)
	{
		if (tab[i])
			free(tab[i]);
		i ++;
	}
	free(tab);
}
