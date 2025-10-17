/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:54:47 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/17 12:28:15 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_list_empty_node(t_token *token)
{
	token->str = NULL;
	token->type = NONE;
	token->kind = UNKNOWN;
	token->next = NULL;
	token->hdoc_fd = -1;
	token->wild_expanded = FALSE;
	token->dollar_expanded = FALSE;
}

t_token	*token_list_add_node(t_token **token_list_start)
{
	t_token	*new_node;
	t_token	*curr_node;

	new_node = malloc(sizeof(t_token));
	if (new_node == NULL)
		return (NULL);
	if (*token_list_start != NULL)
	{
		curr_node = *token_list_start;
		while (curr_node->next)
		{
			curr_node = curr_node->next;
		}
		curr_node->next = new_node;
	}
	else
		*token_list_start = new_node;
	token_list_empty_node(new_node);
	return (new_node);
}

//TODO : token_list_copy_node pour subshell
void	token_list_fill_node(
	t_token *token, char *str, enum e_type type, enum e_kind kind)
{	
	if (!token)
		write(2, "err : trying to reach an empty token node !\n", 45);
	if (!str)
		write(2, "err : trying to save a null string in a token !\n", 49);
	if (!str || !token)
		return ;
	token->str = str;
	token->type = type;
	token->kind = kind;
}

int	token_list_size(t_token *token_list)
{
	int	i;

	i = 0;
	while (token_list)
	{
		i ++;
		if (i == ARG_MAX)
			return (ARG_MAX);
		token_list = token_list->next;
	}
	return (i);
}

void	token_list_insert_list(t_token *token_from, t_token *new_list)
{
	t_token	*pivot;

	pivot = token_from->next;
	token_from->next = new_list;
	while (new_list->next)
		new_list = new_list->next;
	new_list->next = pivot;
}
