/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:02:49 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/17 18:31:24 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	fonctions pour creer et gerer le lexer -> une liste chainee
	des tokens composants l'input
*/

void	lexer_empty_node(t_lexer *node)
{
	node->str = NULL;
	node->token_type = NONE;
	node->next = NULL;
}

t_lexer	*lexer_add_node(t_lexer **lexer_start)
{
	t_lexer	*new_node;
	t_lexer	*curr_node;

	new_node = malloc(sizeof(t_lexer));
	if (new_node == NULL)
		return (NULL);
	if (*lexer_start != NULL)
	{
		curr_node = *lexer_start;
		while (curr_node->next)
		{
			curr_node = curr_node->next;
		}
		curr_node->next = new_node;
	}
	else
	{
		*lexer_start = new_node;
	}
	lexer_empty_node(new_node);
	return (new_node);
}

void	lexer_node_fill(t_lexer *lexer_node, char *str, enum e_token_type type)
{	
	if (!lexer_node)
		write(2, "err : trying to reach an empty lexer node !\n", 45);
	if (!str)
		write(2, "err : trying to save a null string in a lexer !\n", 49);
	if (!str || !lexer_node)
		return;
	lexer_node->str = str;
	lexer_node->token_type = type;
}

int		match_an_operator_pattern(char *str, int length)
{
	if (str_ncmp(str, OR, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, AND, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, PIPE, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, OUT, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, IN, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, HDOC, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, OUT_APP, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, BRACKET_O, length, FALSE))
		return (TRUE);
	if (str_ncmp(str, BRACKET_C, length, FALSE))
		return (TRUE);
	return (FALSE);
}

int		get_operator_type(char *str)
{
	if (str_cmp(str, OR, FALSE))
		return (CONTR_OPERATOR);
	if (str_cmp(str, AND, FALSE))
		return (CONTR_OPERATOR);
	if (str_cmp(str, PIPE,  FALSE))
		return (CONTR_OPERATOR);
	if (str_cmp(str, OUT, FALSE))
		return (REDIR_OPERATOR);
	if (str_cmp(str, IN, FALSE))
		return (REDIR_OPERATOR);
	if (str_cmp(str, HDOC,  FALSE))
		return (REDIR_OPERATOR);
	if (str_cmp(str, OUT_APP, FALSE))
		return (REDIR_OPERATOR);
	if (str_cmp(str, BRACKET_O, FALSE))
		return (BRACKET);
	if (str_cmp(str, BRACKET_C, FALSE))
		return (BRACKET);
	return (FALSE);
}

int		lexer_parse_word(char *input)
{
	int	i;
	
	i = 0;
	while (input[i] && !is_char_operator(input[i]) && !is_char_separator(input[i]))
	{
		if (input[i] == DOUBLE_QUOTE)
		{
			i ++;
			while (input[i] && input[i] != DOUBLE_QUOTE)
				i ++;
			continue;
		}
			if (input[i] == SINGLE_QUOTE)
		{
			i ++;
			while (input[i] && input[i] != SINGLE_QUOTE)
				i ++;
			continue;
		}
		i ++;
	}
	return (i);
}

int		lexer_parse_operator(char *input)
{
	int i;

	i = 0;
	if (input[i] && is_char_operator(input[i]) && !is_char_separator(input[i]))
	{
		if (match_an_operator_pattern(input, i))
		{
			while (input[i] && match_an_operator_pattern(input, i))
			{
				i ++;
			}
			return (i);
		}
		else
		{
			//TO DEL
			printf("syntax error, lexer_parse_operator return -1\n");
			return (-1);
		}
	}
	return (0);
}

// could use type INVALID instead of current logic
void	lexer_register_string(t_lexer **lexer, char **input, int *len, enum e_token_type type)
{
	char	*substr;
	t_lexer	*new_node;

	new_node = lexer_add_node(lexer);
	if (new_node == NULL)
		return;
	if (input == NULL)
	{
		type = NONE;
		substr = NULL;
	}
	else
	{
		substr = extract_string(*input, *len);
		if (substr == NULL)
			return;
		if (type == NONE)
			type = get_operator_type(substr);
	}
	lexer_node_fill(new_node, substr, type);
	if (input)
		*input += *len;
}

//remplacer par un print fd ??
void	print_err(const char *str)
{
	int	i;

	i = ft_strlen(str);
	write(2, str, i);
}
// TODO
// maybe precise word type here ??
int		lexer_is_token_invalid_word(t_lexer *token, char *prev_str, int prev_type)
{
	(void)token;
	(void)prev_type;
	(void)prev_str;
	int	single_quotes;
	int double_quotes;
	int	i;

	if (token->next == NULL)
	{
		i = 0;
		single_quotes = FALSE;
		double_quotes = FALSE;
		while (token->str[i])
		{
			if (token->str[i] == SINGLE_QUOTE && !double_quotes)
				single_quotes = (single_quotes == FALSE);
			if (token->str[i] == DOUBLE_QUOTE && !single_quotes)
				double_quotes = (double_quotes == FALSE);
			i ++;
		}
		if (single_quotes)
		{
			print_err(PROMPT);
			print_err("unexpected EOF while looking for matching \'\'\'\n");
			print_err(PROMPT);
			print_err(" syntax error: unexpected end of file\n");
		}
		if (double_quotes)
		{
			print_err(PROMPT);
			print_err("unexpected EOF while looking for matching \'\"\'\n");
			print_err(PROMPT);
			print_err("syntax error: unexpected end of file\n");
		}
	}
	return (FALSE);
}

int		lexer_is_token_invalid_contr_operator(t_lexer *token, char *prev_str, int prev_type)
{
	(void)token;
	(void)prev_type;
	(void)prev_str;
	return (FALSE);
}

int		lexer_is_token_invalid_redir_operator(t_lexer *token, char *prev_str, int prev_type)
{
	(void)token;
	(void)prev_type;
	(void)prev_str;
	return (FALSE);
}

int		lexer_is_token_invalid_bracket(t_lexer *token, char *prev_str, int prev_type)
{
	(void)token;
	(void)prev_type;
	(void)prev_str;
	return (FALSE);
}

//Not so fan of 2 following print_err
int		lexer_is_token_invalid(t_lexer *token, char *prev_str, int prev_type)
{
	if (token->token_type == NONE)
	{
		print_err(PROMPT);
		print_err("syntax error : invalid character\n;");
	}
	if (token->token_type == WORD)
		return (lexer_is_token_invalid_word(token, prev_str, prev_type));
	if (token->token_type == CONTR_OPERATOR)
		return (lexer_is_token_invalid_contr_operator(token, prev_str, prev_type));
	if (token->token_type == REDIR_OPERATOR)
		return (lexer_is_token_invalid_redir_operator(token, prev_str, prev_type));
	if (token->token_type == BRACKET)
		return (lexer_is_token_invalid_bracket(token, prev_str, prev_type));
	return (FALSE);
}

// can I tragically enter with lexer == NULL ?
void	lexer_syntax_check(t_lexer **lexer)
{
	t_lexer				*curr_token;
	enum e_token_type	prev_type;
	char				*prev_str;
	
	prev_str = NULL;
	prev_type = NONE;
	curr_token = *lexer;
	while (curr_token)
	{
		if (lexer_is_token_invalid(curr_token, prev_str, prev_type))
		{
			clean_lexer(lexer);
			return;
		}
		prev_str = curr_token->str;
		prev_type = curr_token->token_type;
		curr_token = curr_token->next;
	}
}

//	I don't like that i give value to operator in this func and value of words later
void	lexer_parse_input(t_lexer **lexer, char *input) 
{
	int		i;
	char	*substr;

	substr = "command beginning \n";
	while (*input)
	{
		i = lexer_parse_word(input);
		if (i)
			lexer_register_string(lexer, &input, &i, WORD);
		i = lexer_parse_operator(input);
		if (i == -1)
		{
			lexer_register_string(lexer, NULL, 0, NONE);
			break;
		}
		else if (i)
			lexer_register_string(lexer, &input, &i, NONE);
		while (*input && is_char_separator(*input)) //invalid read on first operator
			input ++;
	}
	lexer_syntax_check(lexer);
}

//TODO:
// -> checker types des nodes pour determiner syntax error && word type	
