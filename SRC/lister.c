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

int temp_exec(t_exec **exec_list, t_env **env, t_token **token_list_head)
{
	int temp_res = TRUE;
	pid_t pid;
	int status;
	t_pid_list *pids = NULL;
	int saved_stdin;
	int saved_stdout;
	static int	prev_fd;

	// printf("modifying prev_fd value\n");
	prev_fd = -1;
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (perror("dup"), 1);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
		return (perror("dup"), 1);
	while (*exec_list != NULL)
	{
		if ((*exec_list)->is_subshell)
		{
			// printf("in subshell node\n");
			int pipefds[2] = {-1, -1};
			if (pipe(pipefds) == -1)
				return (perror("pipe"), 1);
			
			pid = fork();
			if (pid == -1)
			{
				if (pipefds[0] != -1)
					close(pipefds[0]);
				if (pipefds[1] != -1)
					close(pipefds[1]);
				return (perror("fork"), 1);
			}

			if (pid == 0)
			{
			// printf("in child subshell\n");

				if (prev_fd != -1)
				{
					// printf("in subshell node, prev_fd is %d\n", prev_fd);
					if (dup2(prev_fd, STDIN_FILENO) == -1)
						exit(1);
					close(prev_fd);
				}
				if (pipefds[1] != -1)
				{
					// printf("in subshell node, setting up pipe for next command at fd %d\n", pipefds[1]);
					if (dup2(pipefds[1], STDOUT_FILENO) == -1)
						exit(1);
					close(pipefds[1]);
				}
				if (!(*exec_list)->next)
				{
					if (dup2(saved_stdout, STDOUT_FILENO) == -1)
						exit(1);
					close(saved_stdout);
				}
				if (pipefds[0] != -1)
					close(pipefds[0]);
				if (pipefds[1] != -1)
					close(pipefds[1]);
				// printf("Launching subshell\n");
				clean_token_list(token_list_head);
				rl_clear_history();
				status = handle_subshell_execution(*exec_list, env);
				// printf("Subshell exited with status %d\n", status);
				clean_env(env);
				exit(status);
			}
			if (prev_fd != -1)
				close(prev_fd);
			if ((*exec_list)->next)
			{
				// printf("Parent process setting up for next command, fd set to %d\n", pipefds[0]);
				prev_fd = pipefds[0];
				if (pipefds[1] != -1)
					close(pipefds[1]);
			}
			else
			{
				if (pipefds[0] != -1)
					close(pipefds[0]);
				if (pipefds[1] != -1)
					close(pipefds[1]);
				prev_fd = -1;
			}
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				temp_res = WEXITSTATUS(status);
			else
				temp_res = status;
		}
		else if ((*exec_list)->is_command)
		{
			if ((!(*exec_list)->next) && is_builtin(*exec_list) == TRUE)
			{
				temp_res = built_in_exec(*exec_list, env);
			}
			else
			{
				status = exec_pipeline(*exec_list, &pids, env, &prev_fd);
				if (!(*exec_list)->next || (*exec_list)->next->is_command == FALSE)
				{
					temp_res = pid_wait_all(pids, status);
					clean_pid(&pids);
				}
			}
		}
		clean_exec_node(exec_list);
	}
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror("dup2 restore stdin");
	close(saved_stdin);
	return (temp_res);
}

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

//TODOLONG
t_token	*scan_subshell_tokens(t_token **token_list)
{
	int		open_bracket;
	t_token	*sublist;
	t_token	*subtoken;
	char	*sub_str;

	open_bracket = 1;
	sublist = NULL;
	*token_list = (*token_list)->next;
	while (1)
	{
		if ((*token_list)->kind == BRACKET_O)
			open_bracket ++;
		if ((*token_list)->kind == BRACKET_C)
		{
			open_bracket --;
			if (!open_bracket)
			{
				*token_list = (*token_list)->next;
				break ;
			}
		}
		subtoken = token_list_add_node(&sublist);
		if (subtoken == NULL)
		{
			clean_token_list(&sublist);
			return (NULL);
		}
		sub_str = ft_strdup((*token_list)->str);
		if (sub_str == NULL)
		{
			free(sub_str);
			clean_token_list(&sublist);
			return (NULL);
		}
		token_list_fill_node(subtoken, sub_str,
			(*token_list)->type, (*token_list)->kind);
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
int	scan_command_tokens(t_token **token_list, t_command *command)
{
	int	in_count;
	int	out_count;
	int	arg_count;

	in_count = 0;
	out_count = 0;
	arg_count = 0;
	while (*token_list && ((*token_list)->type == WORD || (*token_list)->type == REDIR_OPERATOR))
	{
		if ((*token_list)->type == REDIR_OPERATOR)
		{
			command->redir_kind[in_count] = (*token_list)->kind;
			*token_list = (*token_list)->next;
			command->redir[in_count] = (*token_list)->str;
			in_count ++;
		}
		else if ((*token_list)->type == WORD)
		{
			if ((*token_list)->kind == WORD_COM)
			{
				command->argv[0] = ft_strdup((*token_list)->str);
				if (command->argv[0] == NULL)
					return (ERR_MALLOC);
			}
			else if ((*token_list)->kind == WORD_ARG)
			{
				arg_count ++;
				command->argv[arg_count] = ft_strdup((*token_list)->str);
				if (command->argv[arg_count] == NULL)
					return (ERR_MALLOC);
			}
		}
		*token_list = (*token_list)->next;
	}
	return (ERR_SUCCESS);
}

int	build_exec_command_node(t_token **token_list, t_exec **exec_list, t_env *env)
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

int	pipeline_to_exec(t_token **token_list, t_env *env, t_exec **exec_list, int lvalue)
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
	int				sub_fd;

	sub_fd = -1;
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
		err = pipeline_to_exec(&data->token_list, data->env, &exec, status);
		if (err)
		{
			status = 2;
			clean_exec_list(&exec);
			if (err == ERR_MALLOC)
				my_exit(status, &data->env, NULL, &data->token_list_head);
		}
		else if (exec)
		{
			status = temp_exec(&exec, &data->env, &data->token_list_head);
			clean_exec_list(&exec);
		}
	}
	return (cleaner(NULL,NULL, &data->token_list_head), status);
}
