/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:02:49 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/11 16:47:11 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	match_an_operator_pattern(char *str, int length)
{
	if (str_ncmp(str, _OR, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _AND, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _PIPE, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _OUT, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _IN, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _HDOC, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _OUT_APP, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _BRACKET_O, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, _BRACKET_C, length, FALSE))
		return (TRUE);
	return (FALSE);
}

static int	get_operator_type(char *str)
{
	if (str_cmp(str, _OR, TRUE)
		|| str_cmp(str, _AND, TRUE)
		|| str_cmp(str, _PIPE, TRUE))
		return (CONTR_OPERATOR);
	if (str_cmp(str, _OUT, TRUE)
		|| str_cmp(str, _IN, TRUE)
		|| str_cmp(str, _HDOC, TRUE)
		|| str_cmp(str, _OUT_APP, TRUE))
		return (REDIR_OPERATOR);
	if (str_cmp(str, _BRACKET_O, TRUE)
		|| str_cmp(str, _BRACKET_C, TRUE))
		return (BRACKET);
	return (NONE);
}

static int	get_operator_kind(char *str)
{
	if (str_cmp(str, _OR, TRUE))
		return (OR);
	if (str_cmp(str, _AND, TRUE))
		return (AND);
	if (str_cmp(str, _PIPE, TRUE))
		return (PIPE);
	if (str_cmp(str, _OUT, TRUE))
		return (OUT);
	if (str_cmp(str, _IN, TRUE))
		return (IN);
	if (str_cmp(str, _HDOC, TRUE))
		return (HDOC);
	if (str_cmp(str, _OUT_APP, TRUE))
		return (OUT_APP);
	if (str_cmp(str, _BRACKET_O, TRUE))
		return (BRACKET_O);
	if (str_cmp(str, _BRACKET_C, TRUE))
		return (BRACKET_C);
	return (UNKNOWN);
}

static int	lexer_scan_word(char *input)
{
	int	i;

	i = 0;
	while (input[i] && !is_char_operator(input[i])
		&& !is_char_separator(input[i]))
	{
		if (input[i] == DOUBLE_QUOTE)
		{
			i ++;
			while (input[i] && input[i] != DOUBLE_QUOTE)
				i ++;
		}
		if (input[i] == SINGLE_QUOTE)
		{
			i ++;
			while (input[i] && input[i] != SINGLE_QUOTE)
				i ++;
		}
		if (input[i])
			i ++;
	}
	return (i);
}

static int	lexer_scan_operator(char *input)
{
	int	i;

	i = 0;
	if (input[i] && is_char_operator(input[i]) && !is_char_separator(input[i]))
	{
		i ++;
		while (input[i] && match_an_operator_pattern(input, i))
		{
			i ++;
		}
		return (i);
	}
	return (0);
}

// could use type INVALID instead of current logic
void	lexer_create_token_from_input(
	t_token **token_list, char **input, int len, enum e_type type)
{
	char		*substr;
	t_token		*new_node;
	enum e_kind	kind;

	new_node = token_list_add_node(token_list);
	kind = UNKNOWN;
	if (new_node == NULL || input == NULL)
		return ;
	else
	{
		substr = extract_string(*input, len);
		if (substr == NULL)
			return ;
		if (type == NONE)
		{
			type = get_operator_type(substr);
			kind = get_operator_kind(substr);
		}	
	}
	token_list_fill_node(new_node, substr, type, kind);
	if (input)
		*input += len;
}

void	lexer(t_token **token_list, char *input, struct s_data *data)
{
	int		len;
	char	*input_save;

	input_save = input;
	while (*input)
	{
		len = lexer_scan_word(input);
		if (len)
			lexer_create_token_from_input(token_list, &input, len, WORD);
		len = lexer_scan_operator(input);
		if (len == -1)
		{
			lexer_create_token_from_input(token_list, NULL, 0, NONE);
			break ;
		}
		else if (len)
			lexer_create_token_from_input(token_list, &input, len, NONE);
		while (*input && is_char_separator(*input))
			input ++;
	}
	free(input_save);
	data_save_head(data);
}
