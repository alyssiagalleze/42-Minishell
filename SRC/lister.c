/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lister.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:21:10 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/10 15:43:48 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

// void	print_exec_list(t_exec *exec_list)
// {
// 	int	i;

// 	i = 0;

// 	while (exec_list->subshell->token_sublist)
// 	{
// 		printf("arg :%s\n ", exec_list->subshell->token_sublist->str);
// 		exec_list->subshell->token_sublist = exec_list->subshell->token_sublist->next;
// 	}
// }

void	clean_exec_node(t_exec **exec_list)
{
	int	i;
	t_exec	*next;

	i = 0;
	next = (*exec_list)->next;
	if (exec_list && *exec_list)
	{
		if ((*exec_list)->is_command)
		{
			while ((*exec_list)->command->argv[i])
			{
				free((*exec_list)->command->argv[i]);
				(*exec_list)->command->argv[i] = NULL;
				i ++;
			}
			free((*exec_list)->command);
		}
		else if ((*exec_list)->is_subshell)
		{
			if ((*exec_list)->subshell->token_sublist)
				clean_token_list(&((*exec_list)->subshell->token_sublist));
			free((*exec_list)->subshell);
		}
		free(*exec_list);
	}
	*exec_list = next;
}

int subcall_lister(t_exec *exec_list, t_env **env);

/*
*	IMPORTANT : Il faut avancer et clean exec_list ici !!
*
*
*/

int temp_exec(t_exec **exec_list, t_env **env, t_token **token_list_save, int *prev_fd)
{
	
    int temp_res = TRUE;
    pid_t pid;
    int status;
    t_pid_list *pids = NULL;

    while (*exec_list != NULL)
    {
        if ((*exec_list)->is_subshell)
        {
            int pipefds[2] = {-1, -1};
            int need_pipe = ((*exec_list)->next && (*exec_list)->next->is_command);
    
            if (need_pipe && pipe(pipefds) == -1)
                return (perror("pipe"), 1);
    
            pid = fork();
            if (pid == -1)
            {
                if (need_pipe)
                {
                    if (pipefds[0] != -1) close(pipefds[0]);
                    if (pipefds[1] != -1) close(pipefds[1]);
                }
                return (perror("fork"), 1); // TODO : should return
            }
    
            if (pid == 0)
            {
				if (*prev_fd != -1)
                {
					if (dup2(*prev_fd, STDIN_FILENO) == -1)
					_exit(1);
                    close(*prev_fd);
                }
				
                if (need_pipe)
                {
					if (dup2(pipefds[1], STDOUT_FILENO) == -1)
					_exit(1);
                    close(pipefds[0]);
                    close(pipefds[1]);
                }
				
				clean_token_list(token_list_save);
				rl_clear_history();
                status = subcall_lister(*exec_list, env);
                clean_env(env);
                _exit(status); //TODO : fonction interdite
            }
    
            if (*prev_fd != -1)
                close(*prev_fd);
    
            if (need_pipe)
            {
                close(pipefds[1]);
                *prev_fd = pipefds[0]; 
            }
            else
            {
                *prev_fd = -1;
            }
    
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                temp_res = WEXITSTATUS(status);
            else
                temp_res = status;
        }
        else if ((*exec_list)->is_command)
        {
            if ((!(*exec_list)->next) && is_builtin((*exec_list)) == TRUE)
            {
                temp_res = built_in_exec((*exec_list), env);
            }
            else
            {
                status = exec_pipeline((*exec_list), &pids, env, prev_fd);
                if (!(*exec_list)->next || (*exec_list)->next->is_command == FALSE)
                {
                    temp_res = pid_wait_all(pids, status);
                    clean_pid(&pids);
                }
            }
        }
		t_exec *temp;
		temp = (*exec_list)->next;
		clean_exec_node(exec_list);
    }

    return (temp_res);
}

void	lister_init_exec(t_exec *exec)
{
	exec->is_command = FALSE;
	exec->is_subshell = FALSE;
	exec->command = NULL;
	exec->subshell = NULL;
	exec->next = NULL;
}

// Renvoie valide sauf pour "TRUE ||" et "FALSE &&"
// L'operateur doit etre en debut de token list
int	lister_is_valid(t_token *token, int lvalue)
{
	if (token->kind == AND && lvalue)
		return (FALSE);
	if (token->kind == OR && !lvalue)
		return (FALSE);
	return (TRUE);
}

// skip l'entierete d'un subshell, de la parenthese entrante a la parenthese fermante
void	lister_skip_subshell(t_token **token_list)
{
	int	open_bracket;

	open_bracket = 1;
	// skip la parenthese d'entree
	(*token_list) = (*token_list)->next;
	// skip jusqu'a parenthese de sortie correspondante inclue
	while (open_bracket)
	{
		if ((*token_list)->kind == BRACKET_O)
			open_bracket ++;
		if ((*token_list)->kind == BRACKET_C)
			open_bracket --;
		*token_list = (*token_list)->next;
	}
}

// skip tant que c'est des mots et des redirs
void	lister_skip_words_and_redirs(t_token **token_list)
{
	while ((*token_list) && ((*token_list)->type == WORD || (*token_list)->type == REDIR_OPERATOR))
	{
		(*token_list) = (*token_list)->next;
	}
}

// skip une liste qui a ete invalidee
// -> skip tous les tokens jusqu'a arriver sur un && ou un || qui n'appartient pas a un subshell
void	lister_skip_list(t_token **token_list)
{
	// skip l'operateur d'entree
	if ((*token_list)->kind == OR || (*token_list)->kind == AND)
	{
		*token_list = (*token_list)->next;
	}
	// skip jusqu'a l'operateur de sortie
	while (*token_list && (*token_list)->type != CONTR_OPERATOR)
	{
		if ((*token_list)->kind == BRACKET_O)
			lister_skip_subshell(token_list);
		else
			lister_skip_words_and_redirs(token_list);
		if (*token_list && (*token_list)->kind == PIPE)
			*token_list = (*token_list)->next;
	}
}

void	exec_list_init_node(t_exec *exec)
{
	exec->pids = NULL;
	exec->is_command = 0;
	exec->is_subshell = 0;
	exec->command = NULL;
	exec->subshell = NULL;
	exec->next = NULL;
}

t_exec	*exec_list_add_node(t_exec **exec_list_start)
{
	t_exec *new_node;
	t_exec *curr_node;

	new_node = malloc(sizeof(t_exec));
	if (new_node == NULL)
		return (NULL);
	if (*exec_list_start != NULL)
	{
		curr_node = *exec_list_start;
		while (curr_node->next)
		{
			curr_node = curr_node->next;
		}
		curr_node->next = new_node;
	}
	else
	{
		*exec_list_start = new_node;
	}
	exec_list_init_node(new_node);
	return (new_node);
}

t_token	*lister_scan_subshell(t_token **token_list)
{
	int	open_bracket;
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
			if (!open_bracket)							//condition de sortie
			{
				*token_list = (*token_list)->next;
				break;
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
		token_list_fill_node(subtoken, sub_str, (*token_list)->type, (*token_list)->kind);
		*token_list = (*token_list)->next;
	}
	return (sublist);
}

// Rajoute un t_subshell a exec_list
// Fait avancer a liste jusqu'a la parenthese fermante inclue
int	lister_create_exec_from_subshell(t_token **token_list, t_exec **exec_list)
{
	t_exec	*new_exec;
	
	new_exec = exec_list_add_node(exec_list);
	if (new_exec == NULL)
		return (ERR_MALLOC);
	new_exec->is_subshell = TRUE;
	new_exec->subshell = malloc(sizeof(t_subshell));
	if (new_exec->subshell == NULL)
		return (ERR_MALLOC);
	new_exec->subshell->token_sublist = lister_scan_subshell(token_list);
	if (new_exec->subshell->token_sublist == NULL)
		return (ERR_MALLOC);
	return (ERR_SUCCESS);
}

// avancer dans ma token list tant que j'ai WORD ou REDIR OPERATEUR
// (normalement il ne peut y avoir que NULL, OR , OU  et | apres)
// faire les expansions a chaque token
// ajouter les infos dans la struct

int	lister_scan_command(t_token **token_list, t_command *command)
{
	int	in_nbr;
	int	out_nbr;
	int	arg_number;
	
	in_nbr = 0;
	out_nbr = 0;
	arg_number = 0;
	while (*token_list && ((*token_list)->type == WORD || (*token_list)->type == REDIR_OPERATOR))
	{
		if ((*token_list)->type == REDIR_OPERATOR)
		{
			command->redir_kind[in_nbr] = (*token_list)->kind;
			*token_list = (*token_list)->next;
			command->redir[in_nbr] = (*token_list)->str;
			in_nbr ++;
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
				arg_number ++;
				command->argv[arg_number] = ft_strdup((*token_list)->str);
				if (command->argv[arg_number] == NULL)
					return (ERR_MALLOC);
			}
		}
		*token_list = (*token_list)->next;
	}
	return (ERR_SUCCESS);
}

void	exec_list_init_command(t_command *command, int *prev_fd)
{
	int	i;
	
	i = 0;
	command->prev_fd = *prev_fd;
	while (i < ARG_MAX)
	{
		command->argv[i] = 0;
		command->redir[i] = 0;
		command->redir_kind[i] = 0;
		i ++;
	}
}

// Rajoute un t_subshell a exec_list
// Fait avancer a liste jusqu'au prochaine operateur exclus
int	lister_create_exec_from_command(t_token **token_list, t_exec **exec_list, t_env *env, int *prev_fd)
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
	exec_list_init_command(new_exec->command, prev_fd);
	err = lister_expand_command(*token_list, env);
	if (err)
	{
		if (err == ERR_AMBIG)
			return (clean_exec_list(exec_list, FALSE), ERR_AMBIG); //TODO : check if clear exec necessary
		if (err == ERR_MALLOC)
			return (ERR_MALLOC);
	}
	err = lister_scan_command(token_list, new_exec->command);
	if (err == ERR_MALLOC)
		return (ERR_MALLOC);
	return (ERR_SUCCESS);
}

// Le role de cette fonction est de dispatcher la token list en plusieurs listes de commandes a executer
// La difficulte vient du fait qu'il y ait des sublists et des pipes
// -> particulierement du fait qu'il peut y avoir une sublist dans un pipe et un pipe dans une sublist
// Le plus simple est sans doute d'envoyer une liste chainee de commande ?
//
// --> Je dois identifier si je suis dans un pipe 
// -> version naive est d'avancer en skippant les sublists et voir si j'ai un pipe a droite 
//
// --> Je dois travailler commande par commande
//
//
// Je dois aussi recuperer le resutat de l'execution de la list
// et faire les expansions a un moment

int	lister_simple(t_token **token_list, t_env *env, t_exec **exec_list, int lvalue, int *prev_fd) 
{
	enum e_err	err;
	
	if (*token_list && !lister_is_valid(*token_list, lvalue))
	{
		lister_skip_list(token_list);
		return (ERR_SUCCESS);
	}
	if ((*token_list)->kind == AND || (*token_list)->kind == OR)
		*token_list = (*token_list)->next;
	while (*token_list && ((*token_list)->kind != AND && (*token_list)->kind != OR))
	{
		if ((*token_list)->kind == PIPE)
			*token_list = (*token_list)->next;
		if ((*token_list)->kind == BRACKET_O)
			err = lister_create_exec_from_subshell(token_list, exec_list);
		else
			err = lister_create_exec_from_command(token_list, exec_list, env, prev_fd);
		if (err == ERR_MALLOC)
			return (ERR_MALLOC);
		if (err == ERR_AMBIG)
		{
			while (*token_list && ((*token_list)->kind != AND && (*token_list)->kind != OR))
				*token_list = (*token_list)->next;
			return (ERR_AMBIG);
		}
	}
	return (ERR_SUCCESS);
}

// void	subclean_exec_list(t_exec **subexec, t_exec **exec_list)
// {
// 	t_exec	*temp;
// 	int		i;
	
// 	while (exec_list && *exec_list)
// 	{
// 		temp = (*exec_list)->next;
// 		while ((*exec))
// 	}
// }

void	data_reset_pointers(struct s_data *data);

int subcall_lister(t_exec *exec_list, t_env **env)
{
	struct s_data	subdata;
	int				sub_fd;
	
	sub_fd = -1;
	subdata.env = *env;
	data_reset_pointers(&subdata);
	subdata.token_list = exec_list->subshell->token_sublist;
	subdata.token_list_save = subdata.token_list;
	clean_exec_list(&exec_list, TRUE);
	return (lister(&subdata, &sub_fd));
}

//TODO : 5 args

// struct s_data
// {
// 	t_token	*token_list;
// 	t_token *token_list_save;
// 	t_env	*env;
// 	int		prev_fd;
// };

//TODO : BIG : est-ce que j'ai besoin de l'input ici ???
// int	lister(t_token **token_list, t_env **env, char **input, t_token **token_list_save, int *prev_fd)
int	lister(struct s_data *data, int *prev_fd)
{
	int			status;
	t_exec		*exec;
	int			err;

	exec = NULL;
	while (data->token_list)
	{
		err = lister_simple(&data->token_list, data->env, &exec, status, prev_fd);
		if (err)
		{
			status = 2;
			if (err == ERR_MALLOC)
			{
				clean_exec_list(&exec, FALSE);
				my_exit(status, &data->env, NULL, &data->token_list_save);
			}
		}
		if (exec)
		{
			status = temp_exec(&exec, &data->env, &data->token_list_save, prev_fd);
			clean_exec_list(&exec, FALSE); 
		}
	}
	cleaner(NULL,NULL, &data->token_list_save);
	return (status);
}


// nettoyer / ranger code

//heredoc meme si skip ?

// true | (true | (true)) | (true)

