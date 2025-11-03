/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordsplit_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:30:18 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 14:10:57 by tfiette          ###   ########.fr       */
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
