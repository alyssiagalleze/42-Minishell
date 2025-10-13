/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordsplit_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:40:06 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/11 18:02:19 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	ft_to_lower(char c)
{
	if (c >= 'A' && c <= 'Z')
		c -= 'A' - 'a';
	return (c);
}

int	sort_tab_nocase(const char *str_1, const char *str_2)
{
	int	i;
	int	diff;

	i = 0;
	while (str_1[i])
	{
		diff = ft_to_lower(str_1[i]) - ft_to_lower(str_2[i]);
		if (diff)
			return (diff);
		i ++;
	}
	return (str_1[i] - str_2[i]);
}

int	sort_tab_case(const char *str_1, const char *str_2)
{
	int	i;
	int	diff;

	i = 0;
	while (str_1[i])
	{
		diff = ft_to_lower(str_1[i]) - ft_to_lower(str_2[i]);
		if (!diff && (str_1[i] != str_2[i]))
			return (diff);
		i ++;
	}
	return (str_1[i] - str_2[i]);
}

void	swap_ptr(void **ptr1, void **ptr2)
{
	void	*temp;

	temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
}

void	sort_tab(char **tab, int word_count)
{
	int	i;
	int	j;
	int	diff;

	j = word_count - 1;
	i = 0;
	while (i < j)
	{
		diff = sort_tab_nocase(tab[i], tab[i + 1]);
		if (diff > 0)
		{
			swap_ptr((void **)&tab[i], (void **)&tab[i + 1]);
			i = 0;
		}
		else if (!diff && (sort_tab_case(tab[i], tab[i + 1]) > 0))
		{
			swap_ptr((void **)&tab[i], (void **)&tab[i + 1]);
			i = 0;
		}
		else
			i ++;
	}
}
