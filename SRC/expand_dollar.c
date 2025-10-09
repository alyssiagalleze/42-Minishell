/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:12:31 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/09 12:13:36 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int		should_expand_dollar(t_token *token_list)
{
	int	i;

	i = 0;
	if (token_list->type != WORD)
		return (-1);
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
		else if (token_list->str[i] == '$' 
			&& is_expandable_char(token_list->str[i + 1]))
			return (i);
		i ++;
	}
	return (-1);
}

char	*get_expand_dollar_value(char *var_name, t_env *env)
{
	char	*var_value;
	
	var_value = NULL;
	while (env)
	{
		if (str_cmp(env->var_name, var_name + 1, FALSE))
		{
			var_value = env->var_value;
			break;
		}
		env = env->next;
	}
	return (var_value);
}

char	*expand_dollar_restring(char *str, int from, int len, char *substr)
{
	int		sublen;
	int		strlen;
	char	*new_str;
	int		i;

	sublen = ft_strlen(substr);
	strlen = ft_strlen(str);
	new_str = malloc(sizeof(char) * (strlen - len + sublen + 1));
	if (new_str == NULL)
		return (NULL);
	i = -1;
	while (++i < from)
		new_str[i] = str[i];
	i = -1;
	while (++i < sublen)
		new_str[from + i] = substr[i];
	i = -1;
	while (++i < strlen - from - len)
		new_str[from + sublen + i] = str[from + len + i];
	new_str[strlen - len + sublen] = '\0';
	return (new_str);
}

int	expand_dollar(t_token *token_list, t_env *env, int index)
{
	int		len;
	char	*var_name;
	char	*var_value;
	char	*new_str;

	len = 1;
	var_name = NULL;
	while (is_expandable_char(token_list->str[index + len]))
		len ++;
	var_name = extract_string(token_list->str + index, len); // MALLOC
	if (var_name == NULL)
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	var_value = get_expand_dollar_value(var_name, env); // NO MALLOC
	new_str = expand_dollar_restring(token_list->str, index, len, var_value); //MALLOC
	if (new_str == NULL)
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		free(var_name);
		return (ERR_MALLOC);
	}
	else if (is_str_empty_or_null(new_str, 0) && token_list->type == WORD_FILE)
	{
		print_err(PROMPT, PERR_AMBIG, NULL, NULL);
		free(var_name);
		free(new_str);
		return (ERR_AMBIG);
	}
	free(var_name);
	free(token_list->str);
	token_list->str = new_str;
	return (ERR_SUCCESS);
}

int	check_expand_dollar(t_token *token_list, t_env *env)
{
	int	index;
	enum e_err	err;
	
	while (token_list)
	{
		index = should_expand_dollar(token_list);
		if (index == -1)
			token_list = token_list->next;
		else
		{
			err = expand_dollar(token_list, env, index);
			if (err)
				return (err);
		}
	}
	return (ERR_SUCCESS);
}
