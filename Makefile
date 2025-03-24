NAME = minishell

SRCS = \
main.c \
main_checks.c \
init.c \
signal.c \
exec.c \
exec_cmd.c \
exec_helper.c \
export.c \
export_helper.c \
cd.c \
env_functions.c \
echo.c \
unset.c \
execve.c \
execve_helper.c \
ft_exit.c \
heredoc.c \
do_heredoc.c \
save_heredoc.c \
int_list.c \
parsing/char_manager.c \
parsing/dollar_multiarg.c \
parsing/dollar_stupid.c \
parsing/dollar.c \
parsing/error_support.c \
parsing/error.c \
parsing/exit.c \
parsing/extra_free.c \
parsing/inout_remover.c \
parsing/inout_support.c \
parsing/inout.c \
parsing/parsing.c \
parsing/print.c \
parsing/quote_split.c \
parsing/word.c

CFLAGS = -Wall -Wextra -Werror -g

LIBFTA = my_libft/libft.a

all: $(NAME)

$(NAME): $(SRCS)
	make -C my_libft
	if [ ! -d tmp ]; then mkdir tmp; fi
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS) $(LIBFTA) -lreadline

val: all
	clear
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --track-fds=yes ./$(NAME)

vall: all
	clear
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./$(NAME)

clean: libft_clean

fclean: clean libft_fclean
	rm -f $(NAME)
	rm -f ~/.mini_history
	rm -fr tmp

re: fclean all

libft_clean:
	make -C my_libft clean

libft_fclean: libft_clean
	make -C my_libft fclean

run: all
	clear
	./$(NAME)
