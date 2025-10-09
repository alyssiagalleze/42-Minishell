/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_asterisk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:27:05 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/07 17:03:43 by tfiette          ###   ########.fr       */
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
	if (!*pattern && !*str)
		return (TRUE);
	if (*pattern != '*')
	{
		if (*pattern == *str)
			return (match_asterisk_pattern_bt(str + 1, pattern + 1));
		else
			return (FALSE);
	}
	while (*pattern == '*')
	{
		pattern ++;
		if (!*pattern)
			return (TRUE);
	}
 	while (*str)
	{
		if (match_asterisk_pattern_bt(str++, pattern))
			return (TRUE);
	}
	return (FALSE);
}

int	hide_file(char *file, char *pattern)
{
	while (*pattern == '*')
		pattern++;
	if (*file == '.' && *pattern != '.')
		return (TRUE);
	return (FALSE);
}

int	is_dir_only(char **pattern)
{
	char	*ptr;

	ptr = *pattern;
	while (*ptr)
		ptr++;
	ptr--;
	if (ptr >= *pattern)
	{
		if (ptr >= *pattern && *ptr == '/')
		{
			while (ptr >= *pattern && *ptr == '/')
			{
				*ptr = '\0';
				ptr --;
			}
			return (TRUE);
		}
	}
	return (FALSE);
}

int	is_dir(struct dirent *file_info)
{
	if (file_info->d_type == 0 || file_info->d_type == 4)
		return (TRUE);
	return (FALSE);
}

int	expand_asterisk_process(DIR *dir, char **new_str, char *pattern, int token_kind)
{
	struct dirent	*curr_file;
	char			*file_name;
	int				dir_only;
	
	dir_only = is_dir_only(&pattern);
	curr_file = readdir(dir);
	while (curr_file)
	{
		file_name = curr_file->d_name;
		if (match_asterisk_pattern_bt(file_name, pattern))
		{
			if (hide_file(file_name, pattern)
				|| (dir_only == TRUE && !is_dir(curr_file)))
			{
				curr_file = readdir(dir);
				continue;
			}
			if (token_kind == WORD_FILE && *new_str)
			{
				print_err(PROMPT, pattern, ": ", PERR_AMBIG);
				free(*new_str);
				return (ERR_AMBIG);
			}
			*new_str = str_append_sq(*new_str, file_name);
			if (*new_str == NULL)
			{
				print_err(PROMPT, PERR_MALLOC, NULL, NULL);
				return (ERR_MALLOC);
			}
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
		return (print_err(PROMPT, PERR_MALLOC, NULL, NULL), ERR_MALLOC);
	dir = opendir(cwd);
	if (!dir)
	{
		free (cwd);
		return (print_err(PROMPT, PERR_MALLOC, NULL, NULL), ERR_MALLOC);
	}
	err = expand_asterisk_process(dir, &new_str, token_list->str, token_list->kind);
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
