/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:20:51 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/11 16:25:55 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	data_reset_pointers(struct s_data *data)
{
	data->token_list = NULL;
	data->token_list_head = NULL;
}

void	data_save_head(struct s_data *data)
{
	data->token_list_head = data->token_list;
}
