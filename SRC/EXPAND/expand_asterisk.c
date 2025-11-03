/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:27:05 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 12:40:51 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_expand_asterisk(t_token *token_list)
{
	int		i;
	char	c;

	i = 0;
	if (token_list->type != WORD)
		return (FALSE);
	while (token_list->str[i])
	{
		if (token_list->str[i] == '"' || token_list->str[i] == '\'')
		{
			c = token_list->str[i];
			i ++;
			while (token_list->str[i] && token_list->str[i] != c)
				i ++;
			if (!token_list->str[i])
				break ;
		}
		else if (token_list->str[i] == '*')
			return (TRUE);
		i ++;
	}
	return (FALSE);
}

int	exp_asterisk_core(
	int token_kind, char **new_str, char *pattern, char *file_name)
{
	if (token_kind == WORD_FILE && *new_str)
	{
		print_err(PROMPT, pattern, ": ", E_AMBIG);
		free(*new_str);
		return (ERR_AMBIG);
	}
	*new_str = str_append_sq(*new_str, file_name);
	if (*new_str == NULL)
	{
		print_err(PROMPT, E_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	return (ERR_SUCCESS);
}

int	exp_asterisk_loop(DIR *dir, char **new_str, char *pattern, int token_kind)
{
	struct dirent	*curr_file;
	char			*file_name;
	int				dir_only;
	enum e_err		err;

	dir_only = is_dir_only(&pattern);
	curr_file = readdir(dir);
	while (curr_file)
	{
		file_name = curr_file->d_name;
		if (should_expand_file(file_name, pattern, dir_only, curr_file))
		{
			err = exp_asterisk_core(token_kind, new_str, pattern, file_name);
			if (err)
				return (err);
		}
		curr_file = readdir(dir);
	}
	return (ERR_SUCCESS);
}

int	expand_asterisk(t_token *token_list)
{
	char		*cwd;
	DIR			*dir;
	char		*new_str;
	enum e_err	err;

	new_str = NULL;
	cwd = getcwd(0, 0);
	if (!cwd)
		return (print_err(PROMPT, E_MALLOC, NULL, NULL), ERR_MALLOC);
	dir = opendir(cwd);
	if (!dir)
	{
		free (cwd);
		return (print_err(PROMPT, E_MALLOC, NULL, NULL), ERR_MALLOC);
	}
	err = exp_asterisk_loop(dir, &new_str, token_list->str, token_list->kind);
	if (err)
		return (closedir(dir), free(cwd), err);
	if (new_str)
	{
		free(token_list->str);
		token_list->str = new_str;
		token_list->wild_expanded = TRUE;
	}
	return (closedir(dir), free(cwd), ERR_SUCCESS);
}

int	check_expand_asterisk(t_token *token_list)
{
	enum e_err	err;

	while (token_list
		&& (token_list->type == WORD || token_list->type == REDIR_OPERATOR))
	{
		if (should_expand_asterisk(token_list))
		{
			err = expand_asterisk(token_list);
			if (err)
				return (err);
		}
		token_list = token_list->next;
	}
	return (ERR_SUCCESS);
}
