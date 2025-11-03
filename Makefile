NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror -g -MMD  
INCLUDES=-I INCLUDES
D_BUILD=.build/

SRC=	SRC/main.c \
		SRC/loop_utils.c \
		SRC/BUILD_EXEC/build_exec.c \
		SRC/BUILD_EXEC/build_exec_scan.c \
		SRC/BUILD_EXEC/build_exec_utils.c \
		SRC/BUILD_EXEC/build_exec_utils_bis.c \
		SRC/BUILTIN/cd.c \
		SRC/BUILTIN/echo.c \
		SRC/BUILTIN/env.c \
		SRC/BUILTIN/pwd.c \
		SRC/BUILTIN/unset.c \
		SRC/BUILTIN/export.c \
		SRC/BUILTIN/export_bis.c \
		SRC/BUILTIN/exit.c \
		SRC/CLEAN/clean.c \
		SRC/CLEAN/clean_bis.c \
		SRC/CLEAN/clean_ter.c \
		SRC/EXEC/assign_var.c \
		SRC/EXEC/build_paths.c \
		SRC/EXEC/exec.c \
		SRC/EXEC/exec_builtins.c \
		SRC/EXEC/exec_pipeline.c \
		SRC/EXEC/exec_subshell.c \
		SRC/EXEC/exec_utils.c \
		SRC/EXEC/find_command_path.c \
		SRC/EXEC/path_checks.c \
		SRC/EXEC/wait_set_status.c \
		SRC/EXPAND/expand.c \
		SRC/EXPAND/expand_asterisk.c \
		SRC/EXPAND/expand_asterisk_bis.c \
		SRC/EXPAND/expand_dollar.c \
		SRC/EXPAND/expand_dollar_bis.c \
		SRC/EXPAND/expand_wordsplit.c \
		SRC/EXPAND/expand_wordsplit_bis.c \
		SRC/EXPAND/wordsplit_sort.c \
		SRC/EXPAND/wordsplit_utils.c \
		SRC/GNL/get_next_line.c \
		SRC/GNL/get_next_line_utils.c \
		SRC/HEREDOC/heredoc.c \
		SRC/HEREDOC/heredoc_delim.c \
		SRC/HEREDOC/heredoc_expand.c \
		SRC/HEREDOC/heredoc_input.c \
		SRC/HEREDOC/heredoc_utils.c \
		SRC/LEXER/lexer.c \
		SRC/LEXER/lexer_utils.c \
		SRC/LIST/env_list.c \
		SRC/LIST/env_list_utils.c \
		SRC/LIST/exec_list.c \
		SRC/LIST/pid_list.c \
		SRC/LIST/token_list.c \
		SRC/LIST/token_list_bis.c \
		SRC/PARSER/parser.c \
		SRC/PARSER/parser_bis.c \
		SRC/REDIRECT/close_fds.c \
		SRC/REDIRECT/open_fds.c \
		SRC/REDIRECT/open_fds_utils.c \
		SRC/REDIRECT/prepare_pipe.c \
		SRC/REDIRECT/redirections_bis.c \
		SRC/REDIRECT/redirections.c \
		SRC/REDIRECT/subshell_redirections.c \
		SRC/SIGNALS/check_signals.c \
		SRC/SIGNALS/init_sigs.c \
		SRC/SIGNALS/sig_handlers.c \
		SRC/UTILS/data.c \
		SRC/UTILS/error.c \
		SRC/UTILS/split.c \
		SRC/UTILS/string_manip.c \
		SRC/UTILS/string_manip_bis.c \
		SRC/UTILS/string_manip_ter.c \
		SRC/UTILS/string_manip_qua.c \
		SRC/UTILS/utils.c \
		SRC/UTILS/utils_bis.c
		
OBJS=	$(addprefix $(D_BUILD), $(SRC:.c=.o))

RM=rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -lreadline -o  $@ $^

$(D_BUILD)%.o:	%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(D_BUILD)

fclean: clean
	$(RM) $(NAME)

cmake:
	make all
	make clean

re: fclean
	make all

DEPS=	$(addprefix $(D_BUILD), $(SRC:.c=.d))
-include $(DEPS)

.PHONY: clean fclean cmake re all
