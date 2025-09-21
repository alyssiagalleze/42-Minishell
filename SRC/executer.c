/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 19:06:23 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/21 18:10:36 by tfiette          ###   ########.fr       */
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
			cmd_data->argv[i] = token->str;
		token = token->next;
	}
	*token_list = token;
	return ;
}

void	executer_scan_operator(t_token	**token_list, t_cmd_data *cmd_data)
{
	t_token 	*token;

	token = *token_list;
	cmd_data->roperator = NONE;
	if (token && token->type == BRACKET )
	{
		cmd_data->roperator = token->kind;
		token = token->next;
	}
	*token_list = token;
}

//pipeline ????
int	temp_exec(t_cmd_data cmd_data)
{
	printf("temp_exec : %s: ", cmd_data.argv[0]);
	if (str_cmp("TRUE", cmd_data.argv[0], FALSE))
		printf("TRUE\n");
	else if (str_cmp("FALSE", cmd_data.argv[0], FALSE))
		printf("FALSE\n");
	else
		printf("\n");
	return (!str_cmp("TRUE", cmd_data.argv[0], FALSE));
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

//comment je fais avancer la struct ?
int	executer(t_token **token_list, int lvalue, enum e_kind loperator)
{
	printf("executer start from :");
	debug_lexer_print_line(*token_list);
	int 		status;
	t_cmd_data	cmd_data; 
	
	status = 2;
	if ((*token_list)->kind == BRACKET_O)
	{
		executer_scan_operator(token_list, &cmd_data); //juste pour skip
		return (executer(token_list, 0, 0));
	}
	executer_empty_cmd_data(&cmd_data);
	executer_scan_list(token_list, &cmd_data);
	executer_scan_operator(token_list, &cmd_data);
	// debug_lexer_print_line(*token_list);
	printf("%s\n", cmd_data.argv[0]);
	if (cmd_data.argv[0] && executer_is_valid(lvalue, loperator))
	{
		status = temp_exec(cmd_data);
	}
	if (*token_list)
		return (executer(token_list, status, cmd_data.roperator));
	// token = *token_list;
	// status = temp_exec(cmd_data);
	// if (token && token->type == CONTR_OPERATOR)
	// {
	// 	if (str_cmp(token->str,_OR, FALSE))
	// 		return(executer_or(&token->next, status));
	// 	else if (str_cmp(token->str,_AND, FALSE))
	// 		return(executer_and(&token->next, status));
	// 	else if (str_cmp(token->str,_PIPE, FALSE))
	// 		return(executer_pipe(&token->next));
	// }
	return (status); //pas ca qu'il faut return en sortie parenthese
}

//careful ARG_MAX
// faire un truc quand je rentre avec && -> TRUE && (TRUE || TRUE) && TRUE

// faire fonction pour y voir plus clair ??
// faire AST ?