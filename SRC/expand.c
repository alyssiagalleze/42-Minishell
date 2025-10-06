/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:52:26 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/04 20:06:40 by tfiette          ###   ########.fr       */
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

void	sort_tab(char **tab)
{
	(void)tab;
	printf("todo sort\n");
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
			return (FALSE);  // TODO : check correctly freed
		}
		token_list_fill_node(new_node, tab[i], WORD, WORD_ARG);
		i ++;
	}
	free(token_list->str);
	token_list_fill_node(token_list, tab[0], WORD, token_list->kind);
	token_list_insert_list(token_list, new_token_list);
	return (TRUE);
}

int	expand_wordsplit(t_token *token_list)
{
	char	**tab;
	int		word_count;

	tab = wordsplit(&word_count, token_list->str);
	if (tab == NULL)
		return (print_err(PROMPT, "malloc failed\n", NULL, NULL), FALSE);
	if (token_list->wild_expanded)
		sort_tab(tab); // TODO
	if (word_count == 1)
	{
		free(token_list->str);
		token_list->str = tab[0];
	}
	else
	{
		if (!expand_wordsplit_tokenize(token_list, tab, word_count)) //TODO : check I free tab correctly here
			return (FALSE);
	}
	// ws_free_tab(tab);
	free(tab);
	return (TRUE);
}

int	check_expand_wordsplit(t_token *token_list)
{
	while (token_list)
	{
		if (should_expand_wordsplit(token_list))
		{
			if (!expand_wordsplit(token_list))
				return (FALSE);
		}
		token_list = token_list->next;
	}
	return (TRUE);
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
		printf("%s\n", token_list->str);
		token_list = token_list->next;
	}
}

//iterer dans la list
// /!\ attention a ne pas decaler le start
int	lister_expand_command(t_token *token_list, t_env *env)
{
	check_expand_dollar(token_list, env);
	if (!check_expand_asterisk(token_list))
		return (FALSE);								// error check
	if (!check_expand_wordsplit(token_list))		// TODO : malloc failure should stop process ??
		return (FALSE);
	check_expand_unquote(token_list);
	return (TRUE);
}
	
	///////////////////////

// avancer expansion de variable
//
// -> scan pour *
// -> recuperer toutes les chaines et eventuellement les sort
//		-> si une seule chaine, free str et rajouter nouveau dup
//		-> si plusieurs chaines
//			-> if word_file (redir) -> syntax error
//			-> else : rajouter maillons et dup str -> /!\ premier peut-etre commande etc
//
// -> repartir debut
// -> scan pour quotes et end quotes 
//		-> ecrire ce que je trouve entre dans un buffer
//		-> copier le contenu du buffer dans la str de base et ferme avec \0