/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exec_utils_bis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 11:23:12 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 11:29:10 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_scan_token(t_token *token)
{
	if (token && (token->type == WORD || token->type == REDIR_OPERATOR))
		return (TRUE);
	return (FALSE);
}
