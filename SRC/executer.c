/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 19:06:23 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/24 14:21:30 by tfiette          ###   ########.fr       */
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

int pipe_exec(t_cmd_data cmd_data)
{
	pid_t	pid;
	int		status;
	int		exit_status;
	
	status = 2;
	exit_status = 666; // a changer 
	pid = fork();
	if (pid == -1)
		return (status);
	else if (!pid)
	{
		status = temp_exec(cmd_data);
		exit (status);
	}
	return (pid);
}

//pipeline ????
int	temp_exec(t_cmd_data cmd_data)
{
	printf("--> temp_exec : %s: ", cmd_data.argv[0]);
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
	if (loperateur == PIPE)
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
	exit_status = 666; // a changer 
	pid = fork();
	if (pid == -1)
		return (status);
	else if (!pid)
	{
		status = executer(token_list, lvalue, 0, is_subshell, 0);
		exit (status); //need to clean everything somehow
	}
	else
	{
		waitpid(pid, &status, 0);
		printf("\n END SUBSHELL\n\n");
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
	}
	return (exit_status);
}

void	executer_empty_pid_tabs(int	(*pid_tabs)[ARG_MAX])
{
	int	i;

	i = 0;
	while (i < ARG_MAX)
	{
		(*pid_tabs)[i] = 0;
		i ++;
	}
}

// le probleme c'est que je dois garder la lvalue toute a gauche du pipe

// est ce que je le lance pour determiner si c pipe 
// ou une fois que je sais que pipe 
// ou une fois que je sais que pipe et valide ?

// je pense une fois que j'ai determine pipe


int pipe_executer(t_token **token_list, t_cmd_data *cmd_data)
{
	int			pid_tabs[ARG_MAX]; //DEAL WITH ARGMAX !!
	int			i;
	int			j;
	int			status;

	i = 0;
	j = 0;
	status = 2;
	// first pipe logic
	printf("First pipe command\n");
	executer_empty_pid_tabs(&pid_tabs);
	pid_tabs[i++] = pipe_exec(*cmd_data);
	while (1)
	{
		executer_scan_list(token_list, cmd_data);
		executer_scan_operator(token_list, cmd_data);
		if (cmd_data->roperator == PIPE)
		{
			// middle pipe
			printf("Middle pipe command\n");
			pid_tabs[i++] = pipe_exec(*cmd_data);
		}
		else 
		{
			// last pipe
			printf("Last pipe command\n");
			pid_tabs[i++] = pipe_exec(*cmd_data);
			break;
		}
	}
	while (j < i)
	{
		waitpid(pid_tabs[j], &status, 0);
		j ++;
	}
	// interpreter status ?
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}

void	pipe_skipper(t_token **token_list, t_cmd_data *cmd_data)
{
	printf("Skipped\n");
	while (cmd_data->roperator == PIPE)
	{
		if (executer_scan_bracket(token_list))
		{
			executer_skip_sublist(token_list);
			executer_scan_operator(token_list, cmd_data);
		}
		else
		{
			executer_scan_list(token_list, cmd_data);
			executer_scan_operator(token_list, cmd_data);
		}
	}
}

// je vais sans doute devoir remplacer les arguments par une struct
int	executer(t_token **token_list, int lvalue, enum e_kind loperator, int *is_subshell, int is_pipe)
{
	printf("executer start from :");
	debug_lexer_print_line(*token_list);
	int 		status;
	t_cmd_data	cmd_data; 
	(void)is_pipe;
	
	status = lvalue;
	executer_empty_cmd_data(&cmd_data);
	if (executer_scan_bracket(token_list))					//check subshell
	{
		if (executer_is_valid(lvalue, loperator))
			status = fork_executer(token_list, is_subshell, lvalue);
		executer_skip_sublist(token_list);
		executer_scan_operator(token_list, &cmd_data);
	}
	else 
	{
		executer_scan_list(token_list, &cmd_data);
		executer_scan_operator(token_list, &cmd_data);
		if (cmd_data.roperator == PIPE)						//check pipe
		{
			if (executer_is_valid(lvalue, loperator))			//valid pipe logic
				status = pipe_executer(token_list, &cmd_data);
			else												//unvalid pipe skip
				pipe_skipper(token_list, &cmd_data);
		}
		else if (executer_is_valid(lvalue, loperator))    //not a pipe
			status = temp_exec(cmd_data);
	}
	if (*token_list && (*token_list)->kind != BRACKET_C)
		return (executer(token_list, status, cmd_data.roperator, is_subshell, 0));
	return (status); 
}

// - take care of arg max
// - redispatch executer en funcs (executer_subshell, executer_list, executer_pipe)
// - redirections
// - pipe fds