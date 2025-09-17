NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror -g -MMD
INCLUDES=-I INCLUDES
D_BUILD=.build/
# HEADERS=minishell.h

SRC=	SRC/main.c \
		SRC/clean.c \
		SRC/debug.c \
		SRC/lexer.c \
		SRC/string_manip.c \
		SRC/cd.c \
		SRC/echo.c \
		SRC/env.c \
		SRC/my_env_list.c \
		SRC/pwd.c \
		SRC/unset.c \
		SRC/split.c \
		SRC/export.c

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
