# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/20 22:35:15 by majkijew          #+#    #+#              #
#    Updated: 2025/10/05 09:00:04 by tdietz-r         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

OBJDIR = obj

SRC = main.c cleanup.c\
		src/signals.c \
		src/exit_code.c \
		src/parser/lexer/tokanisation.c \
		src/parser/lexer/checker.c \
		src/parser/lexer/checker2.c \
		src/parser/lexer/handler.c \
		src/parser/lexer/handler2.c \
		src/parser/lexer/utils.c \
		src/parser/lexer/init_and_free.c \
		src/parser/lexer/syntax_validation.c \
		src/parser/lexer/token_to_struct.c \
		src/parser/segmentation/segmentation.c \
		src/parser/segmentation/segmentation2.c \
		src/parser/lexer/init.c \
		src/parser/segmentation/expansion.c \
		src/parser/segmentation/expansion_handler.c \
		src/parser/segmentation/segmentation_handler.c \
		src/parser/parser/heredoc_helper.c \
		src/parser/parser/line_reader.c \
		src/parser/parser/heredoc_processor.c \
		src/parser/parser/parsing_start_copy.c \
		src/parser/parser/parsing_start.c\
		src/parser/parser/parsing.c\
		src/parser/parser/parsing4.c\
		src/execution/builtins/builtin_cd.c\
		src/execution/builtins/builtin_commands.c\
		src/execution/builtins/builtin_unset.c\
		src/execution/builtins/bulitin_export.c\
		src/execution/builtins/cd_utils.c\
		src/execution/builtins/export_utils.c\
		src/execution/builtins/utils.c\
		src/execution/enviroment/env_init.c\
		src/execution/enviroment/env_utils.c\
		src/execution/pipes/pipes.c\
		src/execution/pipes/child_in_a_pipe.c\
		src/execution/process/external_p.c\
		src/execution/process/heredoc.c\
		src/execution/process/paths.c\
		src/execution/redirections/redirections.c\
		src/execution/start.c\



OBJS = $(SRC:%.c=$(OBJDIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes
LINKER = -lreadline

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(OBJS) $(LIBFT) -o $(NAME) $(LINKER)
	@echo "Done!"

$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

test: clean
	@echo "Running minishell tester..."
	@bash 42_minishell_tester/tester.sh m
	@echo "Cleaning up tester files..."
	@$(MAKE) cleanup
	@echo "Tester cleanup complete!"

cleanup:
	@echo "Cleaning up tester files..."
	@rm -f tmp_* 0 42 '<' '>' PATH 'PATH=' export unset bla_test chmod 1 -r -f
	@rm -f '%s\n' ls rm touch 'minishe*l' '*a' '*a*a' '**' '***'
	@rm -f *.txt *.log *.out *.err
	@rm -f a.out core core.*
	@rm -f test_* debug_* temp_*
	@echo "Tester cleanup complete!"

.PHONY: all clean fclean re test cleanup		src/execution/start.c
