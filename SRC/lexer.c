/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:02:49 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/14 20:59:13 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	fonctions pour creer et gerer le lexer -> une liste chainee
	des tokens composants l'input
*/


t_lexer	*lexer_add_node(t_lexer **lexer_start)
{
	t_lexer	*new_node;
	t_lexer	*curr_node;

	new_node = malloc(sizeof(t_lexer));
	if (new_node == NULL)
		return (NULL);
	if (*lexer_start != NULL)
	{
		curr_node = *lexer_start;
		while (curr_node->next)
		{
			curr_node = curr_node->next;
		}
		curr_node->next = new_node;
	}
	else
	{
		*lexer_start = new_node;
	}
	new_node->next = NULL;
	// empty_lexer();
	return (new_node);
}

/*
	OBSOLETE : plus besoin de malloc, doit recevoir la chaine et son type(word = 0 / operator = 1)
*/
void	lexer_node_fill(t_lexer *lexer_node, char *str, enum e_token_type type)
{
	char	*str_dup;
	int		i;
	
	if (!lexer_node)
		write(2, "err : trying to reach an empty lexer node !\n", 45);
	if (!str)
		write(2, "err : trying to save a null string in a lexer !\n", 49);
	if (!str || !lexer_node)
		return;
	i = 0;
	while (str[i])
		i ++;
	str_dup = malloc(sizeof(char) * (i + 1)); //plus besoin de malloc
	if (str_dup == NULL)
		return;
	i = 0;
	while (str[i])
	{
		str_dup[i] = str[i];
		i ++;
	}
	str_dup[i] = 0;
	lexer_node->str = str_dup;
}

/*
	WIP:
	il faut encore envoyer les substr dans le lexer
	et separer la fonction en plusieurs sous fonctions
*/
void	lexer_parse_input(t_lexer **lexer, char *input) 
{
	int		i;
	char	*substr;

	while (*input)
	{
		i = 0;
		while (input[i] && !is_char_operator(input[i]) && !is_char_separator(input[i]))
		{
			if (input[i] == '\"')
			{
				i ++;
				while (input[i] && input[i] != '\"')
				{
					i ++;
				}
				if (input[i])
					i ++;
				continue;
			}
			if (input[i] == '\'')
			{
				i ++;
				while (input[i] && input[i] != '\'')
				{
					i ++;
				}
				if (input[i])
					i ++;
				continue;
			}
			i ++;
		}
		if (i)
		{
			substr = extract_string(input, i);
			printf("%s-> word\n", substr);
			input += i;
			i = 0;
		}
		while (input[i] && is_char_operator(input[i]) && !is_char_separator(input[i]))
		{
			i ++;
		}
		if (i)
		{
			substr = extract_string(input, i);
			printf("%s-> operator\n", substr);
			input += i;
		}
		while (input[i] && is_char_separator(input[i]))
		{
			input ++;
		}
	}
	(void)lexer;
}
