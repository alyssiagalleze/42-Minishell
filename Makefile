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
		SRC/env_list.c \
		SRC/error.c	\
		SRC/exec_list.c \
		SRC/exec_builtins.c \
		SRC/exec_pipeline.c \
		SRC/exec_subshell.c \
		SRC/exec_utils.c \
		SRC/exec.c \
		SRC/expand.c \
		SRC/expand_asterisk.c \
		SRC/expand_asterisk_bis.c \
		SRC/expand_dollar.c \
		SRC/expand_wordsplit.c \
		SRC/lexer.c \
		SRC/lister.c \
		SRC/parser.c \
		SRC/parser_bis.c \
		SRC/pid_list.c \
		SRC/pipe_utils.c \
		SRC/redirection_utils.c \
		SRC/redirections.c \
		SRC/signals.c \
		SRC/split.c \
		SRC/string_manip.c \
		SRC/subshell_redirections.c \
		SRC/token_list.c \
		SRC/utils.c \
		SRC/wordsplit_sort.c \
		SRC/wordsplit_utils.c \
		SRC/BUILTIN/cd.c \
		SRC/BUILTIN/echo.c \
		SRC/BUILTIN/env.c \
		SRC/BUILTIN/pwd.c \
		SRC/BUILTIN/unset.c \
		SRC/BUILTIN/export.c \
		SRC/BUILTIN/exit.c 

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
