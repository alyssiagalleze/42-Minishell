/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wordsplit_bis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:09:53 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 14:16:55 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ws_allocate_string(
	char **tab, char *str, int curr_char, int curr_word)
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

static int	ws_fill_string(char *to, char *from, int curr_char)
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
		while (words)
		{
			words--;
			if (tab[words])
				free(tab[words]);
		}
		free(tab);
	}
}

int	wordsplit_allocate_fill_strings(char **tab, char *str, int *word_count)
{
	int	curr_word;
	int	curr_char;

	curr_word = 0;
	curr_char = 0;
	while (curr_word < *word_count)
	{
		curr_char = ws_allocate_string(tab, str, curr_char, curr_word);
		if (curr_char == -1)
		{
			ws_free_tab(tab, *word_count);
			return (FALSE);
		}
		curr_word ++;
	}
	tab[curr_word] = NULL;
	curr_word = 0;
	curr_char = 0;
	while (curr_word < *word_count && tab[curr_word] != NULL)
	{
		curr_char = ws_fill_string(tab[curr_word], str, curr_char);
		curr_word ++;
	}
	return (TRUE);
}
