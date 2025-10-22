/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:29:04 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/22 14:13:14 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_lexer_print_type(t_token *lexer_node)
{
	printf("\n\n--DEBUG LEXER PRINT--\n");
	while (lexer_node)
	{
		printf("%s  // ", lexer_node->str);
		if (lexer_node->type == NONE)
			printf("none\n");
		if (lexer_node->type == WORD)
			printf("word\n");
		if (lexer_node->type == CONTR_OPERATOR)
			printf("control operator\n");
		if (lexer_node->type == REDIR_OPERATOR)
			printf("redirection operator\n");
		if (lexer_node->type == BRACKET)
			printf("bracket\n");
		printf("\n");
		lexer_node = lexer_node->next;
	}
	printf("\n");
}

void	debug_lexer_print_kind(t_token *lexer_node)
{
	printf("\n\n--DEBUG LEXER PRINT--\n");
	while (lexer_node)
	{
		printf("%s  // ", lexer_node->str);
		if (lexer_node->kind == UNKNOWN)
			printf("unknown\n");
		if (lexer_node->kind == WORD_FILE)
			printf("WORD_FILE\n");
		if (lexer_node->kind == WORD_VAR)
			printf("WORD_VAR\n");
		if (lexer_node->kind == WORD_COM)
			printf("WORD_COM\n");
		if (lexer_node->kind == WORD_ARG)
			printf("WORD_ARG\n");
		if (lexer_node->kind == OR)
			printf("OR\n");
		if (lexer_node->kind == AND)
			printf("AND\n");
		if (lexer_node->kind == PIPE)
			printf("PIPE\n");
		if (lexer_node->kind == OUT)
			printf("OUT\n");
		if (lexer_node->kind == IN)
			printf("IN\n");
		if (lexer_node->kind == HDOC)
			printf("HDOC\n");
		if (lexer_node->kind == OUT_APP)
			printf("OUT_APP\n");
		if (lexer_node->kind == BRACKET_O)
			printf("BRACKET_O\n");
		if (lexer_node->kind == BRACKET_C)
			printf("BRACKET_C\n");
		printf("\n");
		lexer_node = lexer_node->next;
	}
	printf("\n");
}

void	debug_lexer_print_line(t_token *lexer_node)
{
	while (lexer_node)
	{
		printf("%s", lexer_node->str);
		lexer_node = lexer_node->next;
		if (lexer_node)
			printf(" ");
	}
	printf("\n");
}

void	debug_print_env_tab(char **env)
{
	printf("debug_print_env_tab\n");
	while (*env)
	{
		printf("%s\n", *env);
		*env += 1;
	}
}

// // void	debug_lexer_print_subline(t_token *lexer_node)
// // {
// // 	int open_bracket;

// // 	open_bracket = 1;
// // 	while (lexer_node)
// // 	{
// // 		//printf("%s", lexer_node->str);
// // 		if (lexer_node->kind == BRACKET_O)
// // 			open_bracket ++;
// // 		if (lexer_node->kind == BRACKET_C)
// // 		{
// // 			open_bracket --;
// // 		}
// // 		lexer_node = lexer_node->next;	
// // 	}
// // 	//printf("\n");
// // }

void	debug_print_env(t_env *env)
{
	while (env)
	{
		printf("var name : %s\n", env->var_name);
		printf("var value : %s\n", env->var_value);
		if (env->is_exported)
			printf("is exported\n");
		else
			printf("is not exported\n");
		env = env->next;
	}
}

// void	debug_print_wordsplit(char **tab)
// {
// 	int	i;

// 	//printf("\n--debug print ws tab--\n");
// 	i = 0;
// 	if (tab == NULL)
// 	{
// 		//printf("tab is NULL \n\n");
// 		return ;
// 	}
// 	while (tab[i])
// 	{
// 		//printf("%s\n", tab[i]);
// 		i ++;
// 	}
// 	//printf("\n");
// }
