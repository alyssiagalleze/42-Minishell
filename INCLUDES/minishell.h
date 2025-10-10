/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:35:10 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/10 14:23:06 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>

# define	MAX_ERROR_LEN	256
# define	ERROR_TOO_LONG	"encountered an error too large to display\n"

// # define	ARG_MAX	2097152 //actually more complicated, 18char per command and take in account PATH
# define	ARG_MAX			4096 //minimum POSIX arg_max
// maybe env max ?

# define	TRUE	1
# define	FALSE	0
# define	PROMPT	"\1\e[1;38;5;82m\2XxxM3g4sh311xxX:\1\e[0;38;5;82m\2 "
# define	RESET_FONT	"\1\e[0m\2"

# define	PERR_ARG_MAX	"Too many tokens in command line. Abort.\n"
# define	PERR_AMBIG		"ambiguous redirect\n"
# define	PERR_MALLOC		"Internal malloc failure. Abort.\n"
# define	PERR_QUOTE		"unexpected EOF while looking for matching `\'\'\n"
# define	PERR_QUOTES		"unexpected EOF while looking for matching `\"'\n"
# define	PERR_BRA		"unexpected EOF while looking for matching `(\'\n"
# define	PERR_STX_EOF	"syntax error: unexpected end of file\n"
# define	PERR_STX_Q		"syntax error near unexpected token `"
# define	PERR_STX_NL		"syntax error near unexpected token `newline'\n"
# define	PERR_STX_BRA	"syntax error near unexpected token `('\n"
# define	PERR_STX_OPE	"syntax error : invalid operator "

//treat all metacharacters or only the one in subject ??
//could also say input invalid if metacharacters we don't use ??
# define	METACHARACTERS	"|&;()<>"
# define	METASEPARATORS	"\t \n"

# define	SINGLE_QUOTE	'\''
# define	DOUBLE_QUOTE	'\"'

# define	OPERATOR_NBR	9
# define	_OR			"||"
# define	_AND		"&&"
# define	_PIPE		"|"
# define	_OUT		">"
# define	_IN			"<"
# define	_HDOC		"<<"
# define	_OUT_APP	">>"
# define	_BRACKET_O	"("
# define	_BRACKET_C	")"

enum	e_type
{
	NONE,
	WORD,
	CONTR_OPERATOR,
	REDIR_OPERATOR,
	BRACKET
};

enum	e_kind
{
	UNKNOWN,
	WORD_FILE,
	WORD_VAR,
	WORD_COM,
	WORD_ARG,
	OR,
	AND,
	PIPE,
	OUT,
	IN,
	HDOC,
	OUT_APP,
	BRACKET_O,
	BRACKET_C
};

enum	e_err
{
	ERR_SUCCESS,
	ERR_MALLOC,
	ERR_AMBIG
};

//STRUCTS

typedef struct s_token
{
	char				*str;
	enum e_type			type;
	enum e_kind			kind;
	struct s_token		*next;
	int					wild_expanded;
}	t_token;

typedef struct s_env_list
{
	char				*var_name;
	char				*var_value;
	int					is_exported;
	struct s_env_list	*next;
}	t_env;

typedef struct s_command
{
	char 				*argv[ARG_MAX];
	char				*redir[ARG_MAX];
	int					prev_fd;
	enum e_kind			redir_kind[ARG_MAX];
} t_command;

typedef struct s_subshell
{
	t_token		*token_sublist;
} t_subshell;

typedef struct s_pid_list
{
	pid_t pid;
	struct s_pid_list *next;
}   t_pid_list;

typedef struct s_exec
{
	int					is_command;
	int					is_subshell;
	t_pid_list			*pids;
	t_command			*command;
	t_subshell			*subshell;
	struct s_exec		*next;
} t_exec;


struct s_data
{
	t_token	*token_list;
	t_token *token_list_save;
	t_env	*env;
	int		prev_fd;
};

// PROTOTYPES

// built-ins
void	ft_putstr_fd(const char *s, int fd);
char	**ft_split(char const *s, char c);
int		unset(char **args, t_env **env);
int		cd(char **args, t_env **env);
int		print_env(t_env **env);
int 	pwd(void);
int		echo(char **args);
int		export(char **args, t_env **env);
void	my_exit(int status, t_env **my_env, char **input, t_token **token_list_save);

//	clean.c
void	clean_input(char **input);
void	clean_env(t_env **env);
void	clean_token_list(t_token	**lexer);
void	exec_cleaner(char **path_tab, char *path);
void	clean_exec_list(t_exec **exec_list, int	is_sublist);
void	cleaner(t_env **my_env, char **input, t_token **token_list);


//	debug.c
void	debug_lexer_print_type(t_token *lexer_node);
void	debug_lexer_print_kind(t_token *lexer_node);
void	debug_lexer_print_line(t_token *lexer_node);
// void	debug_lexer_print_subline(t_token *lexer_node);
void	debug_print_env(t_env *env);
void	debug_print_wordsplit(char **tab);

// env_list.c
t_env   *init_env_list(char **env);
int		update_variable(t_env **env, char *var, char *value);
char	*get_var_value(t_env **env, char *var_name);
void	env_add_node(t_env **top, t_env *node);
t_env   *env_new_node(const char *var_name, const char *var_value, int exported);
int		var_exists(t_env **env, char *name);
int		unset_single(char *arg, t_env **env);
char	**transfer_env(t_env **env);
int	ft_lstsize(t_env *lst);

//	error.c
void	print_err(const char *str1, const char *str2, const char *str3, const char *str4);

// exec
int	exec_pipeline(t_exec *exec_list, t_pid_list **pids, t_env **env, int *prev_fd);
// int		exec_pipeline(t_exec *exec_list, t_pid_list **pids, t_env **env);
int		is_builtin(t_exec *exec_list);
int		built_in_exec(t_exec *exec_list, t_env **env);

// expand.c
int		lister_expand_command(t_token *token_list, t_env *env);
int		is_expandable_char(char c);

// expand_asterisk.c
int		check_expand_asterisk(t_token *token_list);

// expand_dollar.c
int		check_expand_dollar(t_token *token_list, t_env *env);

//	lexer.c
t_token	*token_list_add_node(t_token **lexer_start);
void	token_list_fill_node(t_token *lexer_node, char *str, enum e_type type, enum e_kind kind);
void	lexer(t_token **lexer, char *input);

// lister.c
int	lister(struct s_data *data, int *prev_fd);
// int	lister(t_token **token_list, t_env **env, char **input, t_token **token_list_save);

//	string_manip.c
int		is_white_space(const char c);
int		is_char_operator(const char c);
int		is_char_separator(const char c);
int		is_char_in_string(const char c, const char *str, int accept_null);
int		is_str_empty_or_null(const char *str);
int		ft_strlen(const char *str);
int		str_cmp(char *str1, char *str2, int accept_null);
int		str_ncmp(char *str1, char *str2, int n, int accept_null);
char	*extract_string(const char *start, int len);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*str_append_sq(char *from, char *app);

// parser.c
int		parser(t_token **lexer);
int		parser_check_and_assign_word(t_token **token, int prev_type, int prev_kind, int *line_has_cmd);

// pids
int		pid_wait_all(t_pid_list *list, int status);
void 	clean_pid(t_pid_list **list);
void	pid_add_back(t_pid_list **list, pid_t pid);

// // signals.c
// void sigint_handler(int sig, siginfo_t *info, void *context);
// void init_signals(void);


// token_list.c
void	token_list_empty_node(t_token *token);
t_token	*token_list_add_node(t_token **token_list_start);
void	token_list_fill_node(t_token *token, char *str, enum e_type type, enum e_kind kind);

#endif
