/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 19:06:23 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/22 21:20:27 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	temp_exec(t_cmd_data cmd_data);
int	executer_pipe(t_token **token_list);
int	executer_or(t_token **token_list, int lvalue);
int	executer_and(t_token **token_list, int lvalue);

void	executer_empty_cmd_data(t_cmd_data *cmd_data)  //malloc ??
{
	int	i;

	i = 0;
	while (i < ARG_MAX)
	{
		cmd_data->argv[i++] = 0;
	}
	cmd_data->path = NULL;
	cmd_data->roperator = UNKNOWN;
}

void	executer_scan_list(t_token	**token_list, t_cmd_data *cmd_data)
{
	t_token		*token;
	int			i;

	token = *token_list;
	i = 1;
	while (token && token->type != CONTR_OPERATOR && token->type != BRACKET)
	{
		if (token->kind == WORD_COM)
			cmd_data->argv[0] = token->str;
		else if (token->kind == WORD_ARG)
		{
			cmd_data->argv[i] = token->str;
			i ++;
		}
		token = token->next;
	}
	*token_list = token;
}

void	executer_scan_operator(t_token	**token_list, t_cmd_data *cmd_data)
{
	t_token 	*token;

	token = *token_list;
	cmd_data->roperator = UNKNOWN;
	if (token && (token->type == CONTR_OPERATOR))
	{
		cmd_data->roperator = token->kind;
		token = token->next;
	}
	*token_list = token;
}

int	executer_scan_bracket(t_token	**token_list)
{
	t_token 	*token;

	token = *token_list;
	if (token &&  token->kind == BRACKET_O)
	{
		token = token->next;
		*token_list = token;
		return (TRUE);
	}
	return (FALSE);
}

//pipeline ????
int	temp_exec(t_cmd_data cmd_data)
{
	printf("temp_exec : %s: ", cmd_data.argv[0]);
	if (str_cmp("TRUE", cmd_data.argv[0], FALSE))
	{
		printf("TRUE\n");
		return (FALSE);
	}
	else if (str_cmp("FALSE", cmd_data.argv[0], FALSE))
	{
		printf("FALSE\n");
		return (TRUE);
	}
	else
		printf("\n");
	return (TRUE);
}

int	executer_is_valid(int lvalue, enum e_kind loperateur)
{
	if (loperateur == UNKNOWN)
		return (TRUE);
	if (loperateur == AND && !lvalue)
		return (TRUE);
	if (loperateur == OR && lvalue)
		return (TRUE);
	return (FALSE);
}

void	executer_skip_sublist(t_token **token_list)
{
	int	open_brackets;
	t_token	*token;

	open_brackets = 1;
	token = *token_list;
	while (token && open_brackets != 0)
	{
		if (token->kind == BRACKET_O)
			open_brackets ++;
		else if (token->kind == BRACKET_C)
			open_brackets --;
		token = token->next;
	}
	*token_list = token;
}

int	fork_executer(t_token **token_list, int *is_subshell, int lvalue)
{
	printf("\nSUBSHELL\n");
	debug_lexer_print_line(*token_list);
	pid_t	pid;
	int		status;
	int		exit_status;
	
	status = 2;
	exit_status = 666;
	pid = fork();
	if (pid == -1)
		return (status);
	else if (!pid)
	{
		printf("%p in child\n" , is_subshell);
		status = executer(token_list, lvalue, 0, is_subshell);
		*is_subshell = TRUE;
		*token_list = NULL;
		return (status); //need to clean everything somehow
	}
	else
	{
		printf("%p in parent\n" , is_subshell);
		waitpid(pid, &status, 0);
		printf("\n END SUBSHELL\n\n");
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	return (exit_status);
}

//comment je fais avancer la struct ?
int	executer(t_token **token_list, int lvalue, enum e_kind loperator, int *is_subshell)
{
	printf("executer start from :");
	debug_lexer_print_line(*token_list);
	int 		status;
	t_cmd_data	cmd_data; 
	
	printf("values: lvalue: %d loperator: %d\n", lvalue, loperator);
	status = lvalue;
	executer_empty_cmd_data(&cmd_data);
	if (executer_scan_bracket(token_list))
	{
		printf("%p in executer\n", is_subshell);
		if (executer_is_valid(lvalue, loperator))
			status = fork_executer(token_list, is_subshell, lvalue);
		executer_skip_sublist(token_list);
		executer_scan_operator(token_list, &cmd_data);
	}
	else 
	{
		executer_scan_list(token_list, &cmd_data);
		executer_scan_operator(token_list, &cmd_data);
		if (executer_is_valid(lvalue, loperator))
			// status = exec(cmd_data);
		{
			status = temp_exec(cmd_data);
		}
	}
	if (*token_list && (*token_list)->kind != BRACKET_C)
		return (executer(token_list, status, cmd_data.roperator, is_subshell));
	return (status); //pas ca qu'il faut return en sortie parenthese
}

//careful ARG_MAX