/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:05:29 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 10:51:48 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_get_var_name_length(const char *input)
{
	int	len;

	len = 1;
	if (!is_char_in_string(input[len], "0123456789", FALSE, FALSE))
		while (is_expandable_char(input[len]))
			len++;
	return (len);
}

int	heredoc_expand_get_var_value(
	char **input, int len, t_env *env, char **substr)
{
	t_env	*var;

	if (len == 1)
	{
		*substr = ft_strdup("$");
		if (*substr == NULL)
			return (ERR_MALLOC);
	}
	else
	{
		*substr = extract_string(*input + 1, len - 1);
		if (*substr == NULL)
			return (ERR_MALLOC);
		var = var_exists(&env, *substr);
		*substr = ft_free(*substr);
		if (var == NULL)
			return (ERR_SUCCESS);
		*substr = ft_strdup(var->var_value);
		if (*substr == NULL)
			return (ERR_MALLOC);
	}
	return (ERR_SUCCESS);
}

int	heredoc_expand_input_var(char **input, char **new_input, t_env *env)
{
	int		len;
	char	*substr;

	len = heredoc_get_var_name_length(*input);
	if (heredoc_expand_get_var_value(input, len, env, &substr))
	{
		print_err(PROMPT, E_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	if (substr)
	{
		*new_input = str_append(*new_input, substr);
		if (*new_input == NULL)
		{
			print_err(PROMPT, E_MALLOC, NULL, NULL);
			return (ERR_MALLOC);
		}
	}
	*input += len;
	return (ERR_SUCCESS);
}

int	heredoc_copy_until_dollar(char **input, char **new_input)
{
	int		len;
	char	*substr;

	len = 0;
	while ((*input)[len] && (*input)[len] != '$')
		len++;
	substr = extract_string(*input, len);
	if (substr == NULL)
	{
		print_err(PROMPT, E_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	*new_input = str_append(*new_input, substr);
	if (*new_input == NULL)
	{
		print_err(PROMPT, E_MALLOC, NULL, NULL);
		return (ERR_MALLOC);
	}
	*input += len;
	return (ERR_SUCCESS);
}

int	heredoc_expand_input(char **input, t_env *env)
{
	char	*input_save;
	char	*new_input;

	input_save = *input;
	new_input = NULL;
	while (**input)
	{
		if (**input != '$')
		{
			if (heredoc_copy_until_dollar(input, &new_input))
				return (ft_free(input_save), ft_free(new_input), ERR_MALLOC);
		}
		else if (**input == '$')
		{
			if (heredoc_expand_input_var(input, &new_input, env))
				return (ft_free(input_save), ft_free(new_input), ERR_MALLOC);
		}
	}
	input_save = ft_free(input_save);
	*input = new_input;
	return (ERR_SUCCESS);
}
