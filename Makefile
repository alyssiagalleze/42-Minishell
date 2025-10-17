NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror -g -MMD  
INCLUDES=-I INCLUDES
D_BUILD=.build/
# HEADERS=minishell.h

SRC=	SRC/main.c \
		SRC/clean.c \
		SRC/clean_bis.c \
		SRC/data.c \
		SRC/debug.c \
		SRC/error.c	\
		SRC/fd_utils.c \
		SRC/lexer.c \
		SRC/lister.c \
		SRC/parser.c \
		SRC/parser_bis.c \
		SRC/pipe_utils.c \
		SRC/redirection_utils.c \
		SRC/redirections.c \
		SRC/signals.c \
		SRC/split.c \
		SRC/string_manip.c \
		SRC/subshell_redirections.c \
		SRC/utils.c \
		SRC/BUILTIN/cd.c \
		SRC/BUILTIN/echo.c \
		SRC/BUILTIN/env.c \
		SRC/BUILTIN/pwd.c \
		SRC/BUILTIN/unset.c \
		SRC/BUILTIN/export.c \
		SRC/BUILTIN/exit.c \
		SRC/EXEC/exec.c \
		SRC/EXEC/exec_builtins.c \
		SRC/EXEC/exec_pipeline.c \
		SRC/EXEC/exec_subshell.c \
		SRC/EXEC/exec_utils.c \
		SRC/EXPAND/expand.c \
		SRC/EXPAND/expand_asterisk.c \
		SRC/EXPAND/expand_asterisk_bis.c \
		SRC/EXPAND/expand_dollar.c \
		SRC/EXPAND/expand_wordsplit.c \
		SRC/EXPAND/wordsplit_sort.c \
		SRC/EXPAND/wordsplit_utils.c \
		SRC/LIST/env_list.c \
		SRC/LIST/exec_list.c \
		SRC/LIST/pid_list.c \
		SRC/LIST/token_list.c

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
