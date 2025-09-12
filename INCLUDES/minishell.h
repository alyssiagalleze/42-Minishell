/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:35:10 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/11 19:42:53 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# include <sys/wait.h>
# include <sys/types.h>
# include <unistd.h>

# define	TRUE	1
# define	FALSE	0
# define	PROMPT	"\e[1;38;5;82mXxxM3g4sh311xxX:\e[0;38;5;82m "
# define	RESET_FONT	"\e[0m"


// PROTOTYPES

//	string_manip.c
int	is_char_white_space(const char c);
int is_str_empty_or_null(const char *str);

#endif