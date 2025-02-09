# NAME = minishell

# CC = clang

# CFLAGS = -Wall -Wextra -Werror -I includes/ -I libft/includes/

# LIBFT = -L libft -lft
# READLINE = -lreadline -lncurses  # Add Readline and ncurses libraries here

# HEADER = minishell.h

# BUILTINS = cd echo env exit export pwd unset

# ENV = env get_env sort_env shlvl

# EXEC = bin builtin exec

# MAIN = minishell redir signal

# PARSING = line tokens expansions

# TOOLS = fd free token type expansions parsing

# SRC = $(addsuffix .c, $(addprefix srcs/builtins/, $(BUILTINS))) \
#       $(addsuffix .c, $(addprefix srcs/env/, $(ENV))) \
#       $(addsuffix .c, $(addprefix srcs/exec/, $(EXEC))) \
#       $(addsuffix .c, $(addprefix srcs/main/, $(MAIN))) \
#       $(addsuffix .c, $(addprefix srcs/parsing/, $(PARSING))) \
#       $(addsuffix .c, $(addprefix srcs/tools/, $(TOOLS))) \

# OBJ = $(SRC:c=o)

# all: $(NAME)

# $(NAME): $(OBJ)
# 	@make -C libft/
# 	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(READLINE)  # Add $(READLINE) here

# %.o: %.c
# 	@${CC} ${CFLAGS} -c $< -o $@

# clean:
# 	@make clean -C libft/
# 	@rm -f $(OBJ)

# fclean:
# 	@make fclean -C libft/
# 	@rm -f $(OBJ)
# 	@rm -f $(NAME)

# re: fclean all

# test: all
# 	./minishell

# norm:
# 	norminette $(SRC) includes/$(HEADER)

# .PHONY: clean fclean re test norm

NAME = minishell

CC = clang

CFLAGS = -Wall -Wextra -Werror -I includes/ -I libft/includes/ -fPIE

LIBFT = -L libft -lft
READLINE = -lreadline -lncurses

HEADER = minishell.h

BUILTINS = cd echo env exit export pwd unset

ENV = env get_env sort_env shlvl env2

EXEC = bin builtin exec heredoc heredoc_utils heredoc_utils2 heredoc_utils3

MAIN = minishell redir redir2 signal signals_handler

PARSING = line tokens expansions

TOOLS = fd free token token2 type expansions parsing

SRC = $(addsuffix .c, $(addprefix srcs/builtins/, $(BUILTINS))) \
      $(addsuffix .c, $(addprefix srcs/env/, $(ENV))) \
      $(addsuffix .c, $(addprefix srcs/exec/, $(EXEC))) \
      $(addsuffix .c, $(addprefix srcs/main/, $(MAIN))) \
      $(addsuffix .c, $(addprefix srcs/parsing/, $(PARSING))) \
      $(addsuffix .c, $(addprefix srcs/tools/, $(TOOLS))) \

OBJ = $(SRC:c=o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(READLINE) -pie  # Add -pie here

%.o: %.c
	@${CC} ${CFLAGS} -c $< -o $@

clean:
	@make clean -C libft/
	@rm -f $(OBJ)

fclean:
	@make fclean -C libft/
	@rm -f $(OBJ)
	@rm -f $(NAME)

re: fclean all

test: all
	./minishell

norm:
	norminette $(SRC) includes/$(HEADER)

.PHONY: clean fclean re test norm
