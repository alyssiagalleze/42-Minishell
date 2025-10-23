/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_to_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:21:10 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/10 18:25:08 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	is_and_or(t_token *token_list)
{
	if (!token_list)
		return (FALSE);
	if (token_list->kind == AND || token_list->kind == OR)
		return (TRUE);
	return (FALSE);
}

// Renvoie valide sauf pour "TRUE ||" et "FALSE &&"
// L'operateur doit etre en debut de token list
int	should_exec_pipeline(t_token *token, int lvalue)
{
	if (token->kind == AND && lvalue)
		return (FALSE);
	if (token->kind == OR && !lvalue)
		return (FALSE);
	return (TRUE);
}

void	skip_subshell_tokens(t_token **token_list)
{
	int	open_bracket;

	open_bracket = 1;
	(*token_list) = (*token_list)->next;
	while (open_bracket)
	{
		if ((*token_list)->kind == BRACKET_O)
			open_bracket ++;
		if ((*token_list)->kind == BRACKET_C)
			open_bracket --;
		*token_list = (*token_list)->next;
	}
}

void	skip_command_tokens(t_token **token_list)
{
	while (*token_list)
	{
		if ((*token_list)->type == WORD
			|| (*token_list)->type == REDIR_OPERATOR)
		(*token_list) = (*token_list)->next;
		else
			return ;
	}
}

void	skip_pipeline_tokens(t_token **token_list)
{
	if (is_and_or(*token_list))
		*token_list = (*token_list)->next;
	while (*token_list && (*token_list)->type != CONTR_OPERATOR)
	{
		if ((*token_list)->kind == BRACKET_O)
			skip_subshell_tokens(token_list);
		else
			skip_command_tokens(token_list);
		if (*token_list && (*token_list)->kind == PIPE)
			*token_list = (*token_list)->next;
	}
}

t_token	*create_and_fill_subtoken(
	t_token **sublist, t_token *token)
{
	char	*sub_str;
	t_token	*subtoken;

	subtoken = token_list_add_node(sublist);
	if (subtoken == NULL)
	{
		clean_token_list(sublist);
		return (NULL);
	}
	sub_str = ft_strdup(token->str);
	if (sub_str == NULL)
	{
		free(sub_str);
		clean_token_list(sublist);
		return (NULL);
	}
	token_list_fill_node(subtoken, sub_str, token->type, token->kind);
	return (subtoken);
}

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

//TODOLONG
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

int set_command_as_var_assign(char *var, t_command *command)
{
	command->argv[0] = ft_strdup(var);
	if (command->argv[0] == NULL)
		return (ERR_MALLOC);
	command->is_var = TRUE;
	return (ERR_SUCCESS);
}

//TODO : 5 args
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
	while (*token_list &&
		((*token_list)->type == WORD || (*token_list)->type == REDIR_OPERATOR))
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
	if (command->argv[0] == NULL && var)
		if (set_command_as_var_assign(var, command))
			return (ERR_MALLOC);
	return (ERR_SUCCESS);
}

int	build_exec_command_node(t_token **token_list, t_exec **exec_list, t_env **env)
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

//TODO : clean fd dans pipeline skipped ???
int	pipeline_to_exec(t_token **token_list, t_env **env, t_exec **exec_list, int lvalue)
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

int	handle_subshell_execution(t_exec *exec_list, t_env **env)
{
	struct s_data	subshell_data;

	subshell_data.env = *env;
	data_reset_pointers(&subshell_data);
	subshell_data.token_list = exec_list->subshell->token_sublist;
	subshell_data.token_list_head = subshell_data.token_list;
	exec_list->subshell->token_sublist = NULL;
	clean_exec_list(&exec_list);
	return (token_list_to_exec(&subshell_data));
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
	cleaner(NULL,NULL, &data->token_list_head);
	return (status);
}
