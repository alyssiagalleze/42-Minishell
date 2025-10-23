/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:35:10 by tfiette           #+#    #+#             */
/*   Updated: 2025/10/23 13:34:25 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// # define __USE_POSIX
// # define _POSIX_C_SOURCE 2

# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
// # include <bits/sigaction.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <features.h>
# include <signal.h>

# include "get_next_line.h"

// #define _POSIX_C_SOURCE 199309L


# define	TRUE	1
# define	FALSE	0
# define	PROMPT	"\1\e[1;38;5;82m\2XxxM3g4sh311xxX:\1\e[0;38;5;82m\2 "
# define	RESET_FONT	"\1\e[0m\2"
# define	HDOC_HEADER	"heredoc << "
# define	HDOC_PROMPT	"> "

# define	MAX_ERROR_LEN	256
# define	ERROR_TOO_LONG	"encountered an error too large to display\n"

// # define	ARG_MAX	2097152 //actually more complicated, 18char per command and take in account PATH
# define	ARG_MAX			4096 //minimum POSIX arg_max
// maybe env max ?
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
# define	PERR_STX_HDOC	"syntax error: invalid heredoc delim \n"
# define	PERR_ASSIGN		"malloc error, failed assignement of "
# define	PERR_HDOC		"XxxM3g4sh311xxX: warning: here-document at line "
# define	PERR_EOF		" delimited by end-of-file\n"
# define	PERR_HDOC_EOF	"XxxM3g4sh311xxX: warning: here-document delimited by end-of-file (wanted `"
# define	PERR_HDOC_FDS	"heredoc fds management failed : "

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

//	GLOBAL

extern int		g_signal;

//	ENUMS

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
	ERR_AMBIG,
	ERR_HDOC
};

//STRUCTS

typedef struct s_token
{
	char				*str;
	enum e_type			type;
	enum e_kind			kind;
	struct s_token		*next;
	int					hdoc_fd;
	int					wild_expanded;
	int					dollar_expanded;
}	t_token;

//TODO : replace assigned by local
typedef struct s_env_list
{
	char				*var_name;
	char				*var_value;
	int					is_exported;
	int					is_local;
	struct s_env_list	*next;
}	t_env;

typedef struct s_command
{
	char 				*argv[ARG_MAX];
	char				*redir[ARG_MAX];
	int					prev_fd;
	enum e_kind			redir_kind[ARG_MAX];
	int					hdoc_fd[ARG_MAX];
	int					is_var;
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

struct	s_heredoc
{
	int		heredoc_fds[3];
	char	*exp_delim;
	int		is_quoted;
};

struct s_data
{
	t_token	*token_list;
	t_token *token_list_head;
	t_env	*env;
	int		std_fds[2];
};

struct s_exec_data
{
	pid_t	last_pid;
	int		exec_count;
	int		saved_stds[2];
	int		prev_fd;
	int		is_pipe;
	int		pipefds[2];
};


// PROTOTYPES


int	is_and_or(t_token *token_list);

// built-ins
void	ft_putstr_fd(const char *s, int fd);
char	**ft_split(char const *s, char c);
int		unset(char **args, t_env **env);
int		cd(char **args, t_env **env);
int		print_env(t_env **env);
int 	pwd(void);
int		echo(char **args);
int		export(char **args, t_env **env);
char	**split_into_words(char *input);
void	my_exit(int status, t_env **my_env, char **input, t_token **token_list_head);
void	my_exit_builtin(char **args, struct s_exec_data *exec_data, char **input, t_token **token_list_head);
int		is_string_valid_var(char *str);

// EXEC
// exec_builtins.c
int			is_builtin(t_exec *exec_list);
// pid_t		built_in_exec(t_exec *exec_list, t_env **env);
pid_t  		 built_in_exec(t_exec *exec_list, t_env **env, struct s_exec_data *exec_data);
pid_t		exec_single_builtin(t_exec *exec_list, t_env **env, struct s_exec_data *exec_data);

// exec_pipeline.c
pid_t	exec_pipeline(t_exec *exec_list, t_env **env, struct s_exec_data *exec_data);
int		is_builtin(t_exec *exec_list);

// exec_subshell.c
pid_t	exec_subshell(t_exec *exec_list, struct s_data *data, struct s_exec_data *exec_data);

// exec_utils.c
char	*set_command_path(t_exec *exec_list, t_env **env, int pipefds[2], int saved_fds[2]);

// exec.c
int 	execute_list(t_exec **exec_list, struct s_data *data);

// find_command_path.c
char	*set_command_path(t_exec *exec_list, t_env **env, int pipefds[2], int saved_stds[2]);
char	*get_path_for_command(t_exec *exec_list, t_env **my_env, int pipefds[2], int saved_stds[2]);
char	*dup_cmd_arg(t_exec *exec_list, t_env **env, int pipefds[2], int saved_stds[2]);
char	**get_paths(t_env **env);
char	*append_exec_file(char *cmd_name, char *path);

// find_command_path_bis.c
void	is_executable(char *cmd_path, t_env **env, int pipefds[2], int saved_stds[2]);
void	path_variable_missing(char *cmd_name);
void	file_exists(char *cmd_path, t_env **env, int pipefds[2], int saved_stds[2]);
int	has_slash(char *cmd_name);

//	clean.c  + clean_bis.c
void	clean_input(char **input);
void	clean_env(t_env **env);
void	clean_token_list(t_token	**lexer);
void	exec_cleaner(char **path_tab, char *path);
void	clean_exec_list(t_exec **exec_list);
void	clean_exec_node(t_exec **exec_list);
void	exit_clean(struct s_data *data);
void	cleaner(t_env **my_env, char **input, t_token **token_list);

//	data.c
void	data_reset_pointers(struct s_data *data);
void	data_save_head(struct s_data *data);

//	debug.c
void	debug_lexer_print_type(t_token *lexer_node);
void	debug_lexer_print_kind(t_token *lexer_node);
void	debug_lexer_print_line(t_token *lexer_node);
// void	debug_lexer_print_subline(t_token *lexer_node);
void	debug_print_env(t_env *env);
void	debug_print_env_tab(char **env);
void	debug_print_wordsplit(char **tab);

// env_list.c
int		init_env_list(char **env, t_env **env_list);
int		update_variable(t_env **env, char *var, char *value);
char	*get_var_value(t_env **env, char *var_name);
void	env_add_node(t_env **top, t_env *node);
t_env   *env_new_node(const char *var_name, const char *var_value, int is_exported, int is_local);
t_env	*var_exists(t_env **env, char *name);
int		unset_single(char *arg, t_env **env);
int		transfer_env(t_env **env, char ***new_env);
int		env_size(t_env *lst);

//	error.c
void	print_err(const char *str1, const char *str2, const char *str3, const char *str4);

// exec_list.c
void	exec_list_init_command(t_command *command);
void	exec_list_init_node(t_exec *exec);
t_exec	*exec_list_add_node(t_exec **exec_list_start);

// expand.c
int		expand_command(t_token *token_list, t_env **env);
int		is_expandable_char(char c);
void	expand_unquote(char *str);


// expand_asterisk.c + expand_aterisk_bis
int		check_expand_asterisk(t_token *token_list);
//
int		hide_file(char *file, char *pattern);
int		is_dir_only(char **pattern);
int		is_dir(struct dirent *file_info);
int		should_expand_file(
char	*file_name, char *pattern, int dir_only, struct dirent	*file);
	
// expand_dollar.c
int		check_expand_dollar(t_token *token_list, t_env *env);
	
//expand_wordsplit.c
int		check_expand_wordsplit(t_token *token_list);
	
// fd_utils.c
void close_and_exit(int pipefds_r, int pipefds_w, char **my_env, int status);
int	close_no_exit(int pipefds_r, int pipefds_w, int status);

// heredoc.c
int	heredocs(t_token *token_list, int cmd_count, t_env *env);

// find_command_path
char	*get_path_for_command(t_exec *exec_list, t_env **my_env, int pipefds[2], int saved_stds[2]);
//	lexer.c
t_token	*token_list_add_node(t_token **lexer_start);
void	token_list_fill_node(t_token *lexer_node, char *str, enum e_type type, enum e_kind kind);
void	lexer(t_token **lexer, char *input, struct s_data *data);

// token_list_to_exec.c
int 	handle_subshell_execution(t_exec *exec_list, t_env **env);
int		token_list_to_exec(struct s_data *data);

// pipe_utils.c
void free_env_array(char **envp);
int prepare_pipe(t_exec *exec_list, int pipefds[2]);
int	handle_fork_error(int pipefds[2]);
int	save_std_fds(int *std_in, int *std_out);

// REDIRECT
// close_fds.c
void	close_and_exit(int pipefds_r, int pipefds_w, char **my_env, int status);
int		close_no_exit(int pipefds_r, int pipefds_w, int status);
void	close_fds(int pipefds[2], int saved_stds[2]);

// open_fds.c
int     open_fd_out(int i, t_exec *exec_list, struct s_exec_data *exec_data);
int     open_fd_in(int i, int *h, t_exec *exec_list, struct s_exec_data *exec_data);
void    open_fds(t_exec *exec_list, int *fd_in, int *fd_out, struct s_exec_data *exec_data);

// prepare_pipe.c
int 	prepare_pipe(t_exec *exec_list, int pipefds[2]);
int		handle_fork_error(int pipefds[2]);
void 	free_env_array(char **envp);
void	init_std_fds(struct s_data *subshell_data);
int		save_std_fds(int *std_in, int *std_out);

// redirections_bis.c
int		is_out_redirection(t_exec *exec_list, int i);
int		is_in_redirection(t_exec *exec_list, int i);
void	close_fds(int pipefds[2], int saved_stds[2]);
void	init_std_fds(struct s_data *subshell_data);
void	restore_std_fds(int saved_stds[2]);

// redirections.c
int in_redirections(t_exec *exec_list);
int	out_redirections(t_exec *exec_list);
int	redirect_in(t_exec *exec_list, int *fd_in, int prev_fd);

//	string_manip.c
int		is_white_space(const char c);
int		is_char_operator(const char c);
int		is_char_separator(const char c);
int		is_char_in_string(const char c, const char *str, int accept_null, int give_index);
int		is_str_empty_or_null(const char *str);
int		ft_strlen(const char *str);
int		str_cmp(char *str1, char *str2, int accept_null);
int		str_ncmp(char *str1, char *str2, int n, int accept_null);
char	*extract_string(const char *start, int len);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*str_append(char *from, char *app);
char	*str_append_sq(char *from, char *app);
char	*ft_strchr(const char *s, int c);
int is_only_digit(char *arg);

// subshell_redirections.c
int	sub_redirect_fds(t_exec *exec_list, int pipefds[2], int prev_fd);

// parser.c
int		parser(t_token **lexer);
int		parser_check_and_assign_word(t_token **token, int prev_type, int prev_kind, int *line_has_cmd);

// pids
int		pid_wait_all(int exec_count, pid_t pid);
void 	clean_pid(t_pid_list **list);
void	pid_add_back(t_pid_list **list, pid_t pid);

// redirections.c
int	redirect_fds(t_exec *exec_list, int pipefds[2], struct s_exec_data *exec_data);

// // signals.c
// void sigint_handler(int sig, siginfo_t *info, void *context);
// void init_signals(void);
void	init_readline_signals(void);
void	init_exec_father_signals(void);
void	init_exec_child_signals(void);
void	init_heredoc_signals(void);

// token_list.c
void	token_list_empty_node(t_token *token);
t_token	*token_list_add_node(t_token **token_list_start);
void	token_list_fill_node(t_token *token, char *str, enum e_type type, enum e_kind kind);
int		token_list_size(t_token *token_list);
void	token_list_insert_list(t_token *token_from, t_token *new_list);

// utils.c
char	*ft_itoa(int n);
int		ft_atoi(const char *nptr);
void	*ft_free(void *ptr);
void	ft_close(int *fd);

//	wordsplit_sort.c
void	sort_tab(char **tab, int word_count);

//	wordsplit_utils.c
int		ws_count_words(char *str);
int		ws_allocate_string(char **tab, char *str, int curr_char, int curr_word);
int		ws_fill_string(char *to, char *from, int curr_char);
void	check_wordsplit_failure(char **tab, const int word_count);
void	ws_free_tab(char **tab, int word_count);

#endif
