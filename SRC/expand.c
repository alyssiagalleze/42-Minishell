/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:52:26 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/09 12:14:07 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_expandable_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (TRUE);
	if (c >= '0' && c <= '9')
		return (TRUE);
	if (c == '_')
		return (TRUE);
	return (FALSE);
}

int		should_expand_wordsplit(t_token *token_list) //pas vraiment utile ?
{
	int i;
	
	i = 0;
	while (token_list->str[i])
	{
		if (token_list->str[i] == '"')
		{
			i++;
			while (token_list->str[i] != '"')
				i++;
		}
		else if (token_list->str[i] == '\'')
		{
			i++;
			while (token_list->str[i] != '\'')
				i++;
		}
		if (is_white_space(token_list->str[i]))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int		count_word(char *str)
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

int		ws_allocate_string(char **tab, char *str, int curr_char, int curr_word)
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
	return (str_ptr - str + i);
}

int		ws_fill_string(char *to, char *from, int curr_char)
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
		return;
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

// split qui skip quotes
char	**wordsplit(int *word_count, char *str)
{
	char	**tab;
	int		curr_word;
	int		curr_char;

	curr_word = 0;
	curr_char = 0;
	*word_count = count_word(str);
	tab = malloc(sizeof(*tab) * (*word_count + 1));
	if (tab == NULL)
		return (tab);
	while (curr_word < *word_count)
	{
		curr_char = ws_allocate_string(tab, str, curr_char, curr_word);
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
	check_wordsplit_failure(tab, *word_count);
	return (tab);
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

char	ft_to_lower(char c)
{
	if (c >= 'A' && c <= 'Z')
		c -= 'A' - 'a';
	return (c);
}

int		sort_tab_nocase(const char *str_1, const char *str_2)
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
	return (str_1[i] - str_2[i]); // might segfault
}

int		sort_tab_case(const char *str_1, const char *str_2)
{
	int i;
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
	int		i;
	int 	j;
	int		diff;
	
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

void	token_list_insert_list(t_token *token_from, t_token *new_list)
{
	t_token *pivot;

	pivot = token_from->next;
	token_from->next = new_list;
	while (new_list->next)
		new_list = new_list->next;
	new_list->next = pivot;
}

int	expand_wordsplit_tokenize(t_token *token_list, char **tab, int word_count)
{
	t_token *new_token_list;
	t_token *new_node;
	int	i;

	new_token_list = NULL;
	i = 1;
	while (tab[i])
	{
		new_node = token_list_add_node(&new_token_list);
		if (new_node == NULL)
		{
			ws_free_tab(tab, word_count);
			clean_token_list(&new_token_list);
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

// TODO : maoc abort
int	expand_wordsplit(t_token *token_list)
{
	char	**tab;
	int		word_count;

	tab = wordsplit(&word_count, token_list->str);
	if (tab == NULL)
		return (print_err(PROMPT, PERR_MALLOC, NULL, NULL), ERR_MALLOC);
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

int	check_expand_wordsplit(t_token *token_list)
{
	while (token_list)
	{
		if (should_expand_wordsplit(token_list))
		{
			if (!expand_wordsplit(token_list))
				return (ERR_MALLOC);
		}
		token_list = token_list->next;
	}
	return (ERR_SUCCESS);
}

void	expand_unquote(char *str)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	c = FALSE;
	while(str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && (c == FALSE || c == str[i]))
		{
			if (c == str[i])
				c = FALSE;
			else if (c == FALSE)
				c = str[i];
			i++;
			continue;
		}
		if (str[j] != str[i])
			str[j] = str[i];
		i ++;
		j ++;
	}
	while (j < i)
		str[j++] = '\0';
}

void	check_expand_unquote(t_token *token_list)
{
	while (token_list)
	{
		expand_unquote(token_list->str);
		token_list = token_list->next;
	}
}

// TODO : Stop if malloc failure
int	lister_expand_command(t_token *token_list, t_env *env)
{
	enum e_err	err;
	
	err = check_expand_dollar(token_list, env);
	if (err)
		return (err);
	err = check_expand_asterisk(token_list);
	if (err)
		return (err);
	err = check_expand_wordsplit(token_list);
	if (err)
		return (err);
	check_expand_unquote(token_list);
	return (ERR_SUCCESS);
}
