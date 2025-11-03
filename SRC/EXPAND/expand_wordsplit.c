/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wordsplit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:25:58 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 14:10:18 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_wordsplit_tokenize(t_token *token_list, char **tab, int word_count)
{
	t_token	*new_token_list;
	t_token	*new_node;
	int		i;

	new_token_list = NULL;
	i = 1;
	while (tab[i])
	{
		new_node = token_list_add_node(&new_token_list);
		if (new_node == NULL)
		{
			ws_free_tab(tab, word_count);
			clean_token_list(&new_token_list, TRUE);
			return (ERR_MALLOC);
		}
		token_list_fill_node(new_node, tab[i], WORD, WORD_ARG);
		i ++;
	}
	free(token_list->str);
	token_list_fill_node(token_list, tab[0], WORD, token_list->kind);
	token_list_insert_list(token_list, new_token_list);
	return (ERR_SUCCESS);
}

char	**wordsplit(int *word_count, char *str)
{
	char	**tab;
	int		curr_word;

	curr_word = 0;
	*word_count = ws_count_words(str);
	tab = malloc(sizeof(*tab) * (*word_count + 1));
	if (tab == NULL)
		return (tab);
	while (curr_word <= *word_count)
	{
		tab[curr_word] = NULL;
		curr_word ++;
	}
	if (!wordsplit_allocate_fill_strings(tab, str, word_count))
		return (NULL);
	check_wordsplit_failure(tab, *word_count);
	return (tab);
}

int	expand_wordsplit(t_token *token_list)
{
	char	**tab;
	int		word_count;

	tab = wordsplit(&word_count, token_list->str);
	if (tab == NULL)
		return (print_err(PROMPT, E_MALLOC, NULL, NULL), ERR_MALLOC);
	if (word_count == 1)
	{
		free(token_list->str);
		token_list->str = tab[0];
	}
	else
	{
		if (token_list->wild_expanded)
			sort_tab(tab, word_count);
		if (expand_wordsplit_tokenize(token_list, tab, word_count))
			return (ERR_MALLOC);
	}
	free(tab);
	return (ERR_SUCCESS);
}

int	should_expand_wordsplit(t_token *token_list)
{
	int		i;
	char	c;

	i = 0;
	while (token_list->str[i])
	{
		if (token_list->str[i] == '"' || token_list->str[i] == '\'')
		{
			c = token_list->str[i];
			i++;
			while (token_list->str[i] && token_list->str[i] != c)
				i++;
			if (!token_list->str[i])
				break ;
		}
		if (is_white_space(token_list->str[i]))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int	check_expand_wordsplit(t_token *token_list)
{
	while (token_list
		&& token_list->type != REDIR_OPERATOR && token_list->type != BRACKET)
	{
		if (should_expand_wordsplit(token_list))
		{
			if (expand_wordsplit(token_list))
				return (ERR_MALLOC);
		}
		token_list = token_list->next;
	}
	return (ERR_SUCCESS);
}
