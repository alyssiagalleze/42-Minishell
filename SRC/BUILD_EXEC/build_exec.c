/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lister.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:15:54 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/24 18:32:25 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_and_fill_subtoken(t_token **sublist, t_token *token)
{
	char	*sub_str;
	t_token	*subtoken;

	subtoken = token_list_add_node(sublist);
	if (subtoken == NULL)
	{
		clean_token_list(sublist, TRUE);
		return (NULL);
	}
	sub_str = ft_strdup(token->str);
	if (sub_str == NULL)
	{
		clean_token_list(sublist, TRUE);
		return (NULL);
	}
	token_list_copy_node(token, subtoken);
	subtoken->str = sub_str;
	return (subtoken);
}

int	build_exec_subshell_node(t_token **token_list, t_exec **exec_list)
{
	t_exec	*new_exec;

	new_exec = exec_list_add_node(exec_list);
	if (new_exec == NULL)
		return (ERR_MALLOC);
	new_exec->is_subshell = TRUE;
	new_exec->subshell = malloc(sizeof(t_subshell));
	if (new_exec->subshell == NULL)
		return (ERR_MALLOC);
	new_exec->subshell->token_sublist = scan_subshell_tokens(token_list);
	if (new_exec->subshell->token_sublist == NULL)
		return (ERR_MALLOC);
	return (ERR_SUCCESS);
}

int	build_exec_command_node(
	t_token **token_list, t_exec **exec_list, t_env **env)
{
	t_exec		*new_exec;
	enum e_err	err;

	new_exec = exec_list_add_node(exec_list);
	if (new_exec == NULL)
		return (ERR_MALLOC);
	new_exec->is_command = TRUE;
	new_exec->command = malloc(sizeof(t_command));
	if (new_exec->command == NULL)
		return (ERR_MALLOC);
	exec_list_init_command(new_exec->command);
	err = expand_command(*token_list, env);
	if (err)
	{
		if (err == ERR_AMBIG)
			return (clean_exec_list(exec_list), ERR_AMBIG);
		if (err == ERR_MALLOC)
			return (ERR_MALLOC);
	}
	err = scan_command_tokens(token_list, new_exec->command);
	return (err);
}

int	pipeline_to_exec(
	t_token **token_list, t_env **env, t_exec **exec_list, int lvalue)
{
	enum e_err	err;

	if (*token_list && !should_exec_pipeline(*token_list, lvalue))
		return (skip_pipeline_tokens(token_list), ERR_SUCCESS);
	if (is_and_or(*token_list))
		*token_list = (*token_list)->next;
	while (*token_list && !is_and_or(*token_list))
	{
		if ((*token_list)->kind == PIPE)
			*token_list = (*token_list)->next;
		if ((*token_list)->kind == BRACKET_O)
			err = build_exec_subshell_node(token_list, exec_list);
		else
			err = build_exec_command_node(token_list, exec_list, env);
		if (err)
		{
			if (err == ERR_AMBIG)
				while (*token_list && !is_and_or(*token_list))
					*token_list = (*token_list)->next;
			return (err);
		}
	}
	return (ERR_SUCCESS);
}

int	token_list_to_exec(struct s_data *data)
{
	t_exec	*exec;
	int		status;
	int		err;

	exec = NULL;
	status = 0;
	while (data->token_list)
	{
		err = pipeline_to_exec(&data->token_list, &data->env, &exec, status);
		if (err)
		{
			status = 2;
			clean_exec_list(&exec);
			if (err == ERR_MALLOC)
				my_exit(status, &data->env, NULL, &data->token_list_head);
		}
		else if (exec)
		{
			status = execute_list(&exec, data);
			clean_exec_list(&exec);
		}
	}
	cleaner(NULL, NULL, &data->token_list_head);
	return (status);
}
