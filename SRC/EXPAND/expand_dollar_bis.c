/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:27:46 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 12:43:45 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_expand_dollar_value(char *var_name, t_env *env)
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

static char	*expand_dollar_restring(char *str, int from, int len, char *substr)
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

static int	expand_dollar_check_error(t_token *token_list, char *expanded)
{
	if (expanded == NULL)
	{
		print_err(PROMPT, E_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	else if (is_str_empty_or_null(expanded) && token_list->kind == WORD_FILE)
	{
		print_err(PROMPT, E_AMBIG, NULL, NULL);
		free(expanded);
		return (ERR_AMBIG);
	}
	free(token_list->str);
	token_list->str = expanded;
	return (ERR_SUCCESS);
}

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
		print_err(PROMPT, E_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	var_value = get_expand_dollar_value(var_name, env);
	new_str = expand_dollar_restring(token_list->str, index, len, var_value);
	free(var_name);
	return (expand_dollar_check_error(token_list, new_str));
}
