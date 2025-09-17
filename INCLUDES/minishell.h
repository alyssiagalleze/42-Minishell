/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:35:10 by tfiette           #+#    #+#             */
/*   Updated: 2025/09/17 17:16:13 by agalleze         ###   ########.fr       */
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
# include <errno.h>

# define	TRUE	1
# define	FALSE	0
# define	PROMPT	"\1\e[1;38;5;82m\2XxxM3g4sh311xxX:\1\e[0;38;5;82m\2 "
# define	RESET_FONT	"\1\e[0m\2"

//treat all metacharacters or only the one in subject ??
//could also say input invalid if metacharacters we don't use ??
# define	METACHARACTERS	"|&;()<>"
# define	METASEPARATORS	"\t \n"

enum	e_token_type {WORD, OPERATOR};

// STRUCTS

typedef struct s_syntax_list
{
	char					*str;
	enum e_token_type		token_type;
	struct s_syntax_list	*next;
}	t_lexer;

typedef struct s_env_list
{
	char				*var_name;
	char				*var_value;
	int					is_exported;
	struct s_env_list	*next;
}	t_env;

// PROTOTYPES

//	clean.c
void	clean_input(char **input);
void	clean_lexer(t_lexer	*lexer);
void	clean_env(t_env *env);

//	debug.c
void	debug_lexer_print(t_lexer *lexer_node);

//	lexer.c
t_lexer	*lexer_add_node(t_lexer **lexer_start);
void	lexer_node_fill(t_lexer *lexer_node, char *str);
void	lexer_parse_input(t_lexer **lexer, char *input);

//	string_manip.c
int		is_char_white_space(const char c);
int		is_char_operator(const char c);
int		is_char_separator(const char c);
int		is_char_in_string(const char c, const char *str, int accept_null);
int		is_str_empty_or_null(const char *str);
int		str_cmp(char *str1, char *str2, int accept_null);
char	*extract_string(const char *start, int len);
char	*ft_strdup(const char *s);

// env_list
t_env   *init_env_list(char **env);
int		update_variable(t_env **env, char *var, char *value);
char	*get_var_value(t_env **env, char *var_name);
void	env_add_node(t_env **top, t_env *node);
t_env   *env_new_node(const char *var_name, const char *var_value, int exported);
int		var_exists(t_env **env, char *name);

// built-ins
void	ft_putstr_fd(const char *s, int fd);
char	**ft_split(char const *s, char c);
void	unset(t_env **env, char *name);
int		cd(char *path, t_env **env);
int		print_env(t_env **env);
int 	pwd(void);
int		echo(char **args);
int	export(char **args, t_env **env);

#endif
