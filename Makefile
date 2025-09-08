NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./inc -Iincludes -Ilibs/libft
LDFLAGS = -lreadline

LIBFT_DIR = ./libs/libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

SRC =	src/main.c										\
		src/utils/utils.c								\
		src/utils/utils3.c								\
		src/utils/utils2.c								\
		src/utils/utils4.c								\
		src/exit_codes/exit_codes.c						\
		src/gc/gc_core.c								\
		src/gc/gc_alloc.c								\
		src/gc/gc_string.c								\
		src/signal/signal_handler.c						\
		src/signal/signal_handlers.c					\
		src/lexer/lexer.c 								\
		src/lexer/lexer_handlers.c						\
		src/lexer/lexer_tokens.c						\
		src/lexer/lexer_redirect.c						\
		src/lexer/syntax_validator.c					\
		src/lexer/lexer_utils.c							\
		src/lexer/quote_validator.c						\
		src/merger/adjacent_merger.c 					\
		src/parser/parser.c								\
		src/parser/parser_create.c						\
		src/parser/parser_redirections.c 				\
		src/parser/parser_args.c 						\
		src/parser/parser_args_utils.c 					\
		src/parser/parser_utils.c 						\
		src/parser/parser_pipe.c 						\
		src/env/env.c									\
		src/env/env_array.c								\
		src/env/env_utils.c								\
		src/builts/built_cd.c 							\
		src/builts/built_echo.c 						\
		src/builts/built_env.c 							\
		src/builts/built_exit.c 						\
		src/builts/built_exit_utils.c 					\
		src/builts/built_export.c 						\
		src/builts/built_pwd.c 							\
		src/builts/built_unset.c 						\
		src/builts/built_utils.c 						\
		src/executor/exec_main.c 						\
		src/executor/exec_command.c 					\
		src/executor/exec_builtin.c 					\
		src/executor/exec_external.c 					\
		src/executor/exec_external_process.c 			\
		src/executor/exec_pipe.c 						\
		src/executor/exec_pipe_process.c 				\
		src/executor/exec_redirections.c 				\
		src/executor/exec_utils.c 						\
		src/heredoc/hd_delimiter_processing.c 			\
		src/heredoc/hd_token_parsing.c 					\
		src/heredoc/hd_input_reading.c 					\
		src/heredoc/hd_execution.c 						\
		src/heredoc/hd_continuation.c 					\
		src/expand/expansion_args_main.c 				\
		src/expand/expansion_args_empty.c 				\
		src/expand/expansion_args_process.c 			\
		src/expand/expansion_args_utils.c 				\
		src/expand/expansion_env_basic.c 				\
		src/expand/expansion_env_variables.c 			\
		src/expand/expansion_variable_handlers.c 		\
		src/expand/expansion_loop_helpers.c 			\
		src/expand/expansion_string_basic.c 			\
		src/expand/expansion_variable_specials.c 		\
		src/shell/shell_lexer_validator.c				\
		src/shell/shell_quote_processing.c 				\
		src/shell/shell_parser_handler.c 				\
		src/shell/shell_token_expansion.c				\
		src/shell/shell_command_processor.c				\
		src/shell/shell_utils.c							\

OBJ = $(SRC:.c=.o)

PINK     = \033[95;1m
MAGENTA  = \033[35;1m
HOTPINK  = \033[91;1m
RESET    = \033[0m

all: $(NAME)

HEADERS = inc/builtin.h inc/env.h inc/executor.h inc/exit_codes.h inc/expander.h 	\
		  inc/gc.h inc/heredoc.h inc/lexer.h inc/parser.h 							\
		  inc/shell_stages.h inc/utils.h inc/signal_handler.h

$(NAME): $(OBJ)
	@ echo "\n\t$(MAGENTA)Compilation of $(NAME)... $(RESET)"
	@ $(MAKE) -C $(LIBFT_DIR) > /dev/null 2>&1
	@ $(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) $(LIBFT_LIB)
	@ echo "$(PINK)\n\t Welcome to babyshell!🥳\n$(RESET)"

%.o: %.c $(HEADERS)
	@ $(CC) $(CFLAGS) -c $< -o $@

force:
	@ touch $(SRC)
	@ $(MAKE) all

debug:
	@ echo "Source files: $(SRC)"
	@ echo "Object files: $(OBJ)"
	@ echo "Headers: $(HEADERS)"

clean:
	@ $(MAKE) -C $(LIBFT_DIR) clean > /dev/null 2>&1
	@ rm -f $(OBJ)
	@ find . -name "*.o" -type f -delete 2>/dev/null || true
	@ echo "$(HOTPINK)\n\t Object files removed \n$(RESET)"

fclean: clean
	@ $(MAKE) -C $(LIBFT_DIR) fclean > /dev/null 2>&1
	@ rm -f $(NAME)
	@ echo "$(HOTPINK)\t executable file removed \n$(RESET)"

re: fclean all

.PHONY: all clean fclean re
