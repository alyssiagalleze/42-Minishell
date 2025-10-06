/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:12:31 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/04 20:10:15 by tfiette          ###   ########.fr       */
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

// TODO : find a way to retrive which error (enum SUCCESS, FAILURE MALOC, FAILURE AMBIG etc)
void	expand_dollar(t_token *token_list, t_env *env, int index)
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
		//TODO : malloc error
	}
	var_value = get_expand_dollar_value(var_name, env); // NO MALLOC
	new_str = expand_dollar_restring(token_list->str, index, len, var_value); //MALLOC
	if (new_str == NULL)
	{
		//TODO : malloc error
	}
	if (is_str_empty_or_null(new_str, 0) && token_list->type == WORD_FILE)
	{
		// TODO : if $var = "" -> ambiguous redirect
	}
	free(var_name);
	free(token_list->str);
	token_list->str = new_str;
}

void	check_expand_dollar(t_token *token_list, t_env *env)
{
	int	index;
	
	while (token_list)
		//&& (token_list->type == WORD || token_list->type == REDIR_OPERATOR))
	{
		index = should_expand_dollar(token_list);
		if (index == -1)
			token_list = token_list->next;
		else
		{
			expand_dollar(token_list, env, index);
		}
	}
}
