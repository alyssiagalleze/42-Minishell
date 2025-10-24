/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:02:49 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/24 18:13:25 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

//TODO : lexer shouldn't clean input
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
