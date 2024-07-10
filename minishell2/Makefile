SRCS = src/main.c src/envp.c src/built_ins/built_ins.c src/built_ins/echo.c src/built_ins/unset.c src/built_ins/env.c src/built_ins/pwd.c src/built_ins/export.c get_next_line/get_next_line_utils.c get_next_line/get_next_line.c src/utils/utils.c src/utils/parse_line.c
OBJS = $(SRCS:.c=.o)

LIBFTPRINTF_PATH = ./printf
LIBFTPRINTF = $(LIBFTPRINTF_PATH)/libftprintf.a

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -Iincludes

NAME = minishell

# ANSI escape codes for colors and effects
GREEN = \033[0;32m
YELLOW = \033[1;33m
RED = \033[0;31m
BLUE = \033[0;34m
CYAN = \033[0;36m
MAGENTA = \033[0;35m
RESET = \033[0m
BOLD = \033[1m
BLINK = \033[5m

all: $(NAME)

$(NAME): $(LIBFTPRINTF) $(OBJS)
	@echo "$(YELLOW)Linking object files...$(RESET)"
	@$(CC) $(CFLAGS) $(SRCS) -o $(NAME) $(LIBFTPRINTF) 
	@echo "$(GREEN)Executable $(NAME) created successfully!$(RESET)"
	@echo "$(MAGENTA)✨✨ Compilation Complete! Enjoy! ✨✨$(RESET)"

$(LIBFTPRINTF):
	@echo "$(BLUE)Building libftprintf library...$(RESET)"
	@make -C $(LIBFTPRINTF_PATH) all 
	@echo "$(GREEN)libftprintf library built successfully!$(RESET)"

# Animation for compiling
define compile_animation
	@echo -n "$(CYAN)Compiling: ["
	@for i in `seq 1 25`; do \
		echo -n "="; \
		sleep 0.01; \
	done
	@echo "]$(RESET)"
	@echo "$(BOLD)$(MAGENTA) -> $@ Compiled! $(RESET)"
endef

%.o: %.c
	@echo "$(MAGENTA)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@ > 1
	@$(compile_animation)

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@make -C $(LIBFTPRINTF_PATH) clean > 1
	@$(RM) $(OBJS)
	@echo "$(GREEN)Cleaned successfully!$(RESET)"

fclean: clean
	@echo "$(RED)Removing executable...$(RESET)"
	@make -C $(LIBFTPRINTF_PATH) fclean > /dev/null 2>&1
	@$(RM) $(NAME)
	@echo "$(GREEN)Full clean completed successfully!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
