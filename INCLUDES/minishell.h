/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:35:10 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 14:32:35 by agalleze         ###   ########.fr       */
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

# define TRUE			1
# define FALSE			0
# define PROMPT			"\1\e[1;38;5;82m\2XxxM3g4sh311xxX:\1\e[0;38;5;82m\2 "
# define RESET_FONT		"\1\e[0m\2"
# define HDOC_HEADER	"heredoc << "
# define HDOC_PROMPT	"> "

# define MAX_ERROR_LEN	256
# define ERROR_TOO_LONG	"encountered an error too large to display\n"

# define INT_MIN	-2147483647
# define INT_MAX	2147483647

# define ARG_MAX			4096 //minimum POSIX arg_max
# define E_ARG_MAX	"Too many tokens in command line. Abort.\n"
# define E_AMBIG		"ambiguous redirect\n"
# define E_MALLOC	"Internal malloc failure. Abort.\n"
# define E_QUOTE		"unexpected EOF while looking for matching `\'\'\n"
# define E_QUOTES	"unexpected EOF while looking for matching `\"'\n"
# define E_BRA_O		"unexpected EOF while looking for matching `(\'\n"
# define E_STX_EOF	"syntax error: unexpected end of file\n"
# define E_STX_Q		"syntax error near unexpected token `"
# define E_STX_NL	"syntax error near unexpected token `newline'\n"
# define E_STX_BRA	"syntax error near unexpected token `('\n"
# define E_STX_BRA_C	"syntax error near unexpected token `)'\n"
# define E_STX_OPE	"syntax error : invalid operator "
# define E_STX_HDOC	"syntax error: invalid heredoc delim \n"
# define E_ASSIGN	"malloc error, failed assignement of "
# define E_HDOC		"warning: here-document at line "
# define E_EOF		" delimited by end-of-file\n"
# define E_HDOC_EOF	"warning: here-document delimited by end-of-file (wanted `"
# define E_HDOC_FDS	"heredoc fds management failed : "
# define E_STDIN		"Incident with the STDIN\n"
# define E_STDOUT	"Incident with the STDOUT\n"
# define E_REDIR		"You might have redirected it, which is not supported.\n"

# define METACHARACTERS	"|&;()<>"
# define METASEPARATORS	"\t \n"

# define SINGLE_QUOTE	'\''
# define DOUBLE_QUOTE	'\"'

# define OPERATOR_NBR	9
# define _OR			"||"
# define _AND			"&&"
# define _PIPE			"|"
# define _OUT			">"
# define _IN			"<"
# define _HDOC			"<<"
# define _OUT_APP		">>"
# define _BRACKET_O		"("
# define _BRACKET_C		")"

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
	char				*argv[ARG_MAX];
	char				*redir[ARG_MAX];
	int					prev_fd;
	enum e_kind			redir_kind[ARG_MAX];
	int					hdoc_fd[ARG_MAX];
	int					is_var;
}	t_command;

typedef struct s_subshell
{
	t_token		*token_sublist;
}	t_subshell;

typedef struct s_pid_list
{
	pid_t				pid;
	struct s_pid_list	*next;
}	t_pid_list;

typedef struct s_exec
{
	int					is_command;
	int					is_subshell;
	t_pid_list			*pids;
	t_command			*command;
	t_subshell			*subshell;
	struct s_exec		*next;
}	t_exec;

struct	s_heredoc
{
	int		heredoc_fds[3];
	char	*exp_delim;
	int		is_quoted;
};

struct s_data
{
	t_token	*token_list;
	t_token	*token_list_head;
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
	char	**path_tab;
	t_env	*env;
	t_token	*token_list;
};

//// PROTOTYPES ////

//loop_utils.c
void		check_std_errors(void);
void		save_last_status(int *status, t_env **env);
void		get_input(
				char **input, struct s_data *data, int *status, int *cmd_count);
int			is_longer_than_arg_max(struct s_data *data, int *status);

// BUILD_EXEC //
// build_exec_scan.c
t_token		*scan_subshell_tokens(t_token **token_list);
int			scan_command_tokens(t_token **token_list, t_command *command);
// build_exec_utils.c && build_exec_utils_bis.c
int			is_and_or(t_token *token_list);
int			should_exec_pipeline(t_token *token, int lvalue);
void		skip_pipeline_tokens(t_token **token_list);
int			should_scan_token(t_token *token);
// build_exec.c
int			token_list_to_exec(struct s_data *data);
t_token		*create_and_fill_subtoken(t_token **sublist, t_token *token);

// BUILTIN //
// cd.c
int			cd(char **args, t_env **env);
// echo.c
int			echo(char **args);
// env.c
int			print_env(t_env **env);
// exit.c
void		my_exit(int status, t_env **my_env,
				char **input, t_token **token_list_head);
int			my_exit_builtin(t_exec *exec_list,
				struct s_exec_data *exec_data, char **input);
//export.c & export_bis.c
t_env		*var_exists(t_env **env, char *name);
int			is_string_valid_var(char *str);
int			export(char **args, t_env **env);
// pwd.c
void		ft_putstr_fd(const char *s, int fd);
int			pwd(void);
// unset.c
int			unset_single(char *arg, t_env **env);
int			unset(char **args, t_env **env);

// CLEAN //
// clean.c
void		clean_input(char **input);
void		cleaner(t_env **my_env, char **input, t_token **token_list);
void		clean_env(t_env **env);
void		exit_clean(struct s_data *data);
void		clean_token_list(t_token **token_list, int close_sub_fds);
// clean_bis.c
void		clean_exec_list(t_exec **exec_list);
void		clean_exec_node(t_exec **exec_list);
// clean_ter.c
void		clean_data_close_fds(struct s_exec_data *exec_data,
				t_exec *exec_list, int is_in_child);

// EXEC //
// assign_var.c
pid_t		exec_assign_var(t_exec *exec_list, t_env **env);
int			is_var(t_exec *exec_list);
// build_paths.c
char		**get_paths(t_env **env, int *err_malloc);
char		*append_exec_file(char *cmd_name, char *path, int *err_malloc);
int			is_empty_at_start(struct s_exec_data *exec_data);
int			is_empty_at_end(struct s_exec_data *exec_data);
// exec.c
int			execute_list(t_exec **exec_list, struct s_data *data);
// exec_builtins.c
int			is_builtin(t_exec *exec_list);
pid_t		exec_single_builtin(t_exec *exec_list,
				struct s_exec_data *exec_data);
pid_t		built_in_exec(t_exec *exec_list, struct s_exec_data *exec_data);
void		exec_builtin_in_child(t_exec *exec_list,
				struct s_exec_data *exec_data);
// exec_pipeline.c
pid_t		exec_pipeline(t_exec *exec_list, struct s_exec_data *exec_data);
// exec_subshell.c
pid_t		exec_subshell(t_exec *exec_list,
				struct s_data *data, struct s_exec_data *exec_data);
int			handle_subshell_execution(t_exec *exec_list, t_env **env);
// exec_utils.c
void		malloc_exit(t_exec *exec_list, struct s_exec_data *exec_data);
int			transfer_env(t_env **env, char ***new_env);
// find_command_path.c
char		*set_command_path(t_exec *exec_list, struct s_exec_data *exec_data);
// path_checks.c
int			has_slash(char *cmd_name);
char		*check_access(char *cmd_path, int *first_errno);
// wait_set_status.c
int			pid_wait_all(int exec_count, pid_t last_pid);

// EXPAND //
// expand.c
int			expand_command(t_token *token_list, t_env **env);
int			is_expandable_char(char c);
// expand_dollar.c && expand_dollar_bis.c
int			check_expand_dollar(t_token *token_list, t_env *env);
int			expand_dollar(t_token *token_list, t_env *env, int index);
// expand_asterix.c & expand_asterix_bis.c
int			check_expand_asterisk(t_token *token_list);
int			is_dir_only(char **pattern);
int			should_expand_file(char *file_name,
				char *pattern, int dir_only, struct dirent *file);
// expand_wordsplit.c && expand_wordsplit_bis
int			check_expand_wordsplit(t_token *token_list);
int			wordsplit_allocate_fill_strings(
				char **tab, char *str, int *word_count);
void		check_wordsplit_failure(char **tab, const int word_count);

// wordsplit_sort.c
void		sort_tab(char **tab, int word_count);
// wordsplit_utils.c
void		ws_free_tab(char **tab, int word_count);
int			ws_count_words(char *str);

// HEREDOCS //
// herdoc_delim.c
int			heredoc_unquote_delim(char *delim,
				struct s_heredoc *hdoc_data, int *err);
// heredoc_expand.c
int			heredoc_expand_input(char **input, t_env *env);
// heredoc.c
int			heredocs(t_token *token_list, int cmd_count, t_env *env);
// heredoc_input.c
int			heredoc_input_to_pipe(struct s_heredoc *hdoc_data,
				int *cmd_count, t_env *env, int *err);
// heredoc_utils.c
void		reset_hdoc_data(struct s_heredoc *hdoc_data);
void		init_hdoc_data(struct s_heredoc *hdoc_data);
t_token		*skip_until_heredoc_delim(t_token *token_list);
void		heredocs_display_header(char *delim);

// LEXER //
// lexer.c
void		lexer(t_token **token_list, char *input, struct s_data *data);
// lexer_utils.c
int			match_an_operator_pattern(char *str, int length);
int			get_operator_type(char *str);
int			get_operator_kind(char *str);

// LIST //
// env_list.c
t_env		*env_new_node(const char *var_name,
				const char *var_value, int is_exported, int is_local);
void		env_add_node(t_env **top, t_env *node);
int			init_env_list(char **env, t_env **env_list);
// env_list_utils.c
int			update_variable(t_env **env, char *var, char *value);
char		*get_var_value(t_env **env, char *var_name);
int			env_size(t_env *lst);
// exec_list.c
void		exec_list_init_command(t_command *command);
t_exec		*exec_list_add_node(t_exec **exec_list_start);
// token_list.c && token_list_bis.c
t_token		*token_list_add_node(t_token **token_list_start);
void		token_list_copy_node(t_token *from, t_token *to);
void		token_list_fill_node(t_token *token,
				char *str, enum e_type type, enum e_kind kind);
int			token_list_size(t_token *token_list);
void		token_list_insert_list(t_token *token_from, t_token *new_list);

// PARSER //
// parser.c & parser_bis.c
int			parser(t_token **token_list,
				t_token **token_list_head, int *status);
int			parser_check_and_assign_word(t_token **token,
				int prev_type, int prev_kind, int *line_has_cmd);
int			parser_clean_failure(t_token **token_list_head,
				int *status, int error_id);

// REDIRECT //
// close_fds.c
void		close_fds(int pipefds[2], int saved_stds[2]);
// open_fds.c & open_fd_utils.c
int			is_in_redirection(t_exec *exec_list, int i);
int			is_out_redirection(t_exec *exec_list, int i);
int			open_fds(t_exec *exec_list,
				int *fd_in, int *fd_out, struct s_exec_data *exec_data);
int			handle_open_error(t_exec *exec_list,
				struct s_exec_data *exec_data, int i);
int			open_hdoc_fds(t_exec *exec_list, int *hdoc_index);
int			open_fd_out(int i,
				t_exec *exec_list, struct s_exec_data *exec_data);

// prepare_pipe.c
int			save_std_fds(int *std_in, int *std_out);
void		free_env_array(char **envp);
int			handle_fork_error(int pipefds[2]);
int			prepare_pipe(t_exec *exec_list, int pipefds[2]);
// redirections.c & redirections_bis.c
int			in_redirections(t_exec *exec_list);
int			out_redirections(t_exec *exec_list);
void		restore_std_fds(int saved_stds[2]);
int			redirect_fds(t_exec *exec_list,
				int pipefds[2], struct s_exec_data *exec_data);
int			redirect_in(t_exec *exec_list, int *fd_in, int prev_fd);
int			redirect_out(t_exec *exec_list, int *fd_out, int pipefd_out);

// SIGNALS //
// init_sigs.c
void		init_readline_signals(void);
void		init_heredoc_signals(void);
void		init_exec_father_signals(void);
void		init_exec_child_signals(void);
// sig_handlers.c
void		sa_readline_handler(int sig);
void		sa_heredoc_handler(int sig);
void		sa_exec_child_handler(int sig);
// check_signals.c
void		prev_command_check_signal(int *status);
int			heredoc_check_signal(struct s_data *data, int *status);

// UTILS //
// data.c
void		data_reset_pointers(struct s_data *data);
void		data_save_head(struct s_data *data);
// split.c
char		**ft_split(char const *s, char c);
// string_manip.c & bis.c & ter.c & quad.c
int			is_white_space(const char c);
int			is_str_empty_or_null(const char *str);
int			is_char_in_string(const char c,
				const char *str, int accept_null, int give_index);
int			is_char_operator(const char c);
int			is_char_separator(const char c);
int			str_cmp(char *str1, char *str2, int accept_null);
int			ft_strlen(const char *str);
int			str_ncmp(char *str1, char *str2, int n, int accept_null);
char		*ft_strchr(const char *s, int c);
int			only_digit_no_overflow(char *arg);
char		*ft_strjoin(char *s1, char *s2);
char		*str_append_sq(char *from, char *app);
char		*extract_string(const char *start, int len);
char		*str_append(char *from, char *app);
// error.c
void		print_err(const char *str1,
				const char *str2, const char *str3, const char *str4);
// utils.c & utils_bis.c
char		*ft_itoa(int n);
int			ft_atoi(const char *nptr);
void		ft_close(int *fd);
void		*ft_free(void *ptr);
long long	ft_atoll(const char *nptr);

#endif
