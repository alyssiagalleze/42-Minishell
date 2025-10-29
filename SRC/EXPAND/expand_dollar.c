/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:12:31 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/29 13:16:06 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	should_expand_dollar(t_token *token_list)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	str = token_list->str;
	if (token_list->type != WORD)
		return (-1);
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i ++;
			j = i;
			while (str[i] && str[i] != '\'')
				i ++;
			if (!str[i])
			{
				i = j;
				continue ;
			}
		}
		else if (str_ncmp(str + i, "$?", 2, FALSE))
			return (i);
		else if (str[i] == '$' && is_expandable_char(str[i + 1]))
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
			break ;
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

int	expand_dollar_check_error(t_token *token_list, char *expanded)
{
	if (expanded == NULL)
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	else if (is_str_empty_or_null(expanded) && token_list->kind == WORD_FILE)
	{
		print_err(PROMPT, PERR_AMBIG, NULL, NULL);
		free(expanded);
		return (ERR_AMBIG);
	}
	free(token_list->str);
	token_list->str = expanded;
	return (ERR_SUCCESS);
}

void	dollar_expand_unquote(char *str)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (str[i] == '"')
			{
				i ++;
				continue ;
			}
		}
		if (str[j] != str[i])
			str[j] = str[i];
		i ++;
		j ++;
	}
	while (j < i)
		str[j++] = '\0';
}

//TODO : echo "$var'"$var'$var'
//TODO : reprendre expand depuis le caractere apres
int	expand_dollar(t_token *token_list, t_env *env, int index)
{
	int		len;
	char	*var_name;
	char	*var_value;
	char	*new_str;

	len = 1;
	var_name = NULL;
	if (str_ncmp(token_list->str + index, "$?", 2, FALSE))
		len = 2;
	else
		while (is_expandable_char(token_list->str[index + len]))
			len ++;
	var_name = extract_string(token_list->str + index, len);
	if (var_name == NULL)
	{
		print_err(PROMPT, PERR_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	var_value = get_expand_dollar_value(var_name, env);
	new_str = expand_dollar_restring(token_list->str, index, len, var_value);
	free(var_name);
	return (expand_dollar_check_error(token_list, new_str));
}

int	check_expand_dollar(t_token *token_list, t_env *env)
{
	int			index;
	enum e_err	err;

	while (token_list && token_list->type != CONTR_OPERATOR
		&& token_list->type != BRACKET)
	{
		index = should_expand_dollar(token_list);
		if (index == -1)
			token_list = token_list->next;
		else
		{
			if (!token_list->dollar_expanded)
			{
				dollar_expand_unquote(token_list->str);
				token_list->dollar_expanded = TRUE;
				continue ;
			}
			err = expand_dollar(token_list, env, index);
			if (err)
				return (err);
		}
	}
	return (ERR_SUCCESS);
}
