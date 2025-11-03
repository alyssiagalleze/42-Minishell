/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:18:08 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 14:20:03 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_list_insert_list(t_token *token_from, t_token *new_list)
{
	t_token	*pivot;

	pivot = token_from->next;
	token_from->next = new_list;
	while (new_list->next)
		new_list = new_list->next;
	new_list->next = pivot;
}
