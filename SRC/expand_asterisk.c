/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:27:05 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/04 14:42:18 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	should_expand_asterisk(t_token *token_list)
{
	int	i;

	i = 0;
	if (token_list->type != WORD)
		return (FALSE);
	while (token_list->str[i])
	{
		if (token_list->str[i] == '"')
		{
			i ++;
			while (token_list->str[i] != '"')
				i ++;
		}
		else if (token_list->str[i] == '\'')
		{
			i ++;
			while (token_list->str[i] != '\'')
				i ++;
		}
		else if (token_list->str[i] == '*')
			return (TRUE);
		i ++;
	}
	return (FALSE);
}

int	match_asterisk_pattern_bt(char *str, char *pattern)
{
	if (!pattern[0] && !str[0])
		return (TRUE);
	if (pattern[0] != '*') 			// letter in pattern
	{
		if (pattern[0] == str[0]) 	// match
			return (match_asterisk_pattern_bt(str + 1, pattern + 1));
		else						// no match
			return (FALSE);
	}
	while (pattern[0] == '*')
	{
		pattern ++;
		if (!pattern[0])
			return (TRUE);
	}
	while (str[0])
	{
		if (match_asterisk_pattern_bt(str++, pattern))
			return (TRUE);
	}
	return (FALSE);
}

// TODO : check if directories only
int	expand_asterisk_process(DIR *dir, char **new_str, char *old_str, int token_kind)
{
	struct dirent	*curr_file;
	char			*file_name;
	
	curr_file = readdir(dir);
	while (curr_file)
	{
		file_name = curr_file->d_name;
		if (file_name[0] != '.' && match_asterisk_pattern_bt(file_name, old_str))
		{
			if (token_kind == WORD_FILE && *new_str)
			{
				print_err(PROMPT, old_str, ": ", "ambiguous redirect\n");
				free(*new_str);
				return (FALSE);
			}
			*new_str = str_append_sq(*new_str, file_name);
			if (*new_str == NULL)
			{
				print_err(PROMPT, "malloc failed\n", NULL, NULL);
				return (FALSE);
			}
		}
		curr_file = readdir(dir);
	}
	return (TRUE);
}

int	expand_asterisk(t_token *token_list)
{
	char	*cwd;
	DIR		*dir;
	char	*new_str;
	
	new_str = NULL;
	cwd = getcwd(0, 0);
	if (!cwd)
		return (print_err(PROMPT, "malloc failed\n", NULL, NULL), FALSE);
	dir = opendir(cwd);
	if (!dir)
	{
		free (cwd);
		return (print_err(PROMPT, "malloc failed\n", NULL, NULL), FALSE);
	}
	if (!expand_asterisk_process(dir, &new_str, token_list->str, token_list->kind))
	{
		return (closedir(dir), free(cwd), FALSE);
	}
	if (new_str)
	{
		free(token_list->str);
		token_list->str = new_str;
		token_list->wild_expanded = TRUE;
	}
	return (closedir(dir), free(cwd), TRUE);
}

int	check_expand_asterisk(t_token *token_list)
{
	while (token_list
		&& (token_list->type == WORD || token_list->type == REDIR_OPERATOR))
	{
		if (should_expand_asterisk(token_list))
		{
			if (!expand_asterisk(token_list))
				return (FALSE);
		}
		token_list = token_list->next;
	}
	return (TRUE);
}
