/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exec_scan.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:27:48 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 11:29:02 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*scan_subshell_tokens(t_token **token_list)
{
	int		open_bracket;
	t_token	*sublist;
	t_token	*subtoken;

	open_bracket = 1;
	sublist = NULL;
	*token_list = (*token_list)->next;
	while (1)
	{
		if ((*token_list)->kind == BRACKET_O)
			open_bracket ++;
		if ((*token_list)->kind == BRACKET_C)
			open_bracket --;
		if (!open_bracket)
		{
			*token_list = (*token_list)->next;
			break ;
		}
		subtoken = create_and_fill_subtoken(&sublist, *token_list);
		if (!subtoken)
			return (NULL);
		*token_list = (*token_list)->next;
	}
	return (sublist);
}

void	scan_command_redirs(
	t_token **token_list, t_command *command, int *redir_count, int	*hdoc_count)
{
	int	is_hdoc;

	is_hdoc = FALSE;
	command->redir_kind[*redir_count] = (*token_list)->kind;
	if ((*token_list)->kind == HDOC)
		is_hdoc = TRUE;
	*token_list = (*token_list)->next;
	command->redir[*redir_count] = (*token_list)->str;
	*redir_count += 1;
	if (is_hdoc)
	{
		command->hdoc_fd[*hdoc_count] = (*token_list)->hdoc_fd;
		*hdoc_count += 1;
	}
}

int	scan_command_words(
	t_token **token_list, t_command *command, int *arg_count, char **var)
{
	if ((*token_list)->kind == WORD_VAR)
		*var = (*token_list)->str;
	else if ((*token_list)->kind == WORD_COM)
	{
		command->argv[0] = ft_strdup((*token_list)->str);
		if (command->argv[0] == NULL)
			return (ERR_MALLOC);
	}
	else if ((*token_list)->kind == WORD_ARG)
	{
		*arg_count += 1;
		command->argv[*arg_count] = ft_strdup((*token_list)->str);
		if (command->argv[*arg_count] == NULL)
			return (ERR_MALLOC);
	}
	return (ERR_SUCCESS);
}

int	scan_var_assign(char *var, t_command *command)
{
	command->argv[0] = ft_strdup(var);
	if (command->argv[0] == NULL)
		return (ERR_MALLOC);
	command->is_var = TRUE;
	return (ERR_SUCCESS);
}

int	scan_command_tokens(t_token **token_list, t_command *command)
{
	int			redir_count;
	int			hdoc_count;
	int			arg_count;
	char		*var;
	enum e_err	err;

	redir_count = 0;
	hdoc_count = 0;
	arg_count = 0;
	var = NULL;
	while (should_scan_token(*token_list))
	{
		if ((*token_list)->type == REDIR_OPERATOR)
			scan_command_redirs(token_list, command, &redir_count, &hdoc_count);
		else if ((*token_list)->type == WORD)
		{
			err = scan_command_words(token_list, command, &arg_count, &var);
			if (err)
				return (err);
		}
		*token_list = (*token_list)->next;
	}
	if (command->argv[0] == NULL && var && scan_var_assign(var, command))
		return (ERR_MALLOC);
	return (ERR_SUCCESS);
}
