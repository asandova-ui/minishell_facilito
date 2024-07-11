SRCS = src/ft_printf.c src/format_handlers/ft_itoabase.c src/format_handlers/ft_uitoa.c src/format_handlers/print_character.c src/format_handlers/print_integer.c src/format_handlers/print_highhex.c src/format_handlers/print_lowhex.c src/format_handlers/print_nosign.c src/format_handlers/print_ptrhexa.c src/format_handlers/print_string.c

OBJS = $(SRCS:.c=.o)

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -Iincludes

NAME = libftprintf.a

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

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(YELLOW)Creating archive file...$(RESET)"
	@cp $(LIBFT) $(NAME) > /dev/null 2>&1
	@ar rcs $(NAME) $(OBJS) > /dev/null 2>&1
	@echo "$(GREEN)Archive file $(NAME) created successfully!$(RESET)"
	@echo "$(MAGENTA)✨✨ Compilation Complete! Enjoy! ✨✨$(RESET)"

$(LIBFT):
	@echo "$(BLUE)Building libft library...$(RESET)"
	@make -C $(LIBFT_PATH) all > /dev/null 2>&1
	@echo "$(GREEN)libft library built successfully!$(RESET)"

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
	@$(CC) $(CFLAGS) -c $< -o $@ > /dev/null 2>&1
	@$(compile_animation)

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@make -C $(LIBFT_PATH) clean > /dev/null 2>&1
	@$(RM) $(OBJS)
	@echo "$(GREEN)Cleaned successfully!$(RESET)"

fclean: clean
	@echo "$(RED)Removing archive and executable...$(RESET)"
	@make -C $(LIBFT_PATH) fclean > /dev/null 2>&1
	@$(RM) $(NAME)
	@echo "$(GREEN)Full clean completed successfully!$(RESET)"

re: fclean all

.PHONY: all clean fclean re libft
