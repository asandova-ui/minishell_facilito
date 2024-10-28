SRCS = src/main.c src/custom_error.c src/initialize_program.c src/philo.c src/rutina.c src/utils.c src/free.c src/eat.c src/eat2.c src/sleep.c src/utils2.c

OBJS = $(SRCS:.c=.o)

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -Iincludes

NAME = philosophers

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

$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking object files...$(RESET)"
	@$(CC) $(CFLAGS) $(SRCS) -o $(NAME) > /dev/null 2>&1
	@echo "$(GREEN)Executable $(NAME) created successfully!$(RESET)"
	@echo "$(MAGENTA)✨✨ Compilation Complete! Enjoy! ✨✨$(RESET)"

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
	@$(RM) $(OBJS)
	@echo "$(GREEN)Cleaned successfully!$(RESET)"

fclean: clean
	@echo "$(RED)Removing executable...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(GREEN)Full clean completed successfully!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
