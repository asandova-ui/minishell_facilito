SRCS = src/main.c src/custom_error.c src/init_things.c src/parsing.c src/exit.c \
		src/color_textures.c src/free_things.c src/init_things_2.c\
		src/mlx_create.c src/parsing_data.c src/read_file.c src/valid_map.c \
		src/valid_map_2.c src/valid_textures.c src/create_map.c src/utils.c \
		get_next_line/get_next_line.c get_next_line/get_next_line_utils.c\
		src/render/render.c src/render/minimap_image.c src/render/minimap_render.c \
		src/render/raycasting.c src/render/texture.c src/render/image_utils.c \
		src/movement/input_handler.c src/movement/player_dir.c src/movement/player_move.c \
		src/movement/player_pos.c src/movement/player_rotate.c

OBJS = $(SRCS:.c=.o)

LIBFTPRINTF_PATH = ./printf
LIBFTPRINTF = $(LIBFTPRINTF_PATH)/libftprintf.a

MLX_PATH = ./mmllxx
MLX = $(MLX_PATH)/libmlx.a

BONUS = 0

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -I$(MLX_PATH) -DBONUS=$(BONUS)
LDFLAGS = -L$(MLX_PATH) -lmlx -L/usr/lib -lXext -lX11 -lm -lz

NAME = cub3d

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

$(NAME): $(LIBFTPRINTF) $(MLX) $(OBJS)
	@echo "$(YELLOW)Linking object files and libraries...$(RESET)"
	$(CC) $(OBJS) $(LIBFTPRINTF) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)Executable $(NAME) created successfully!$(RESET)"
	@echo "$(MAGENTA)✨✨ Compilation Complete! Enjoy! ✨✨$(RESET)"

$(LIBFTPRINTF):
	@echo "$(BLUE)Building libftprintf library...$(RESET)"
	@make -C $(LIBFTPRINTF_PATH) all
	@echo "$(GREEN)libftprintf library built successfully!$(RESET)"

$(MLX):
	@echo "$(BLUE)Building MLX library...$(RESET)"
	@make -C $(MLX_PATH) all
	@echo "$(GREEN)MLX library built successfully!$(RESET)"

# Animation for compiling
define compile_animation
	@echo -n "$(CYAN)Compiling: ["
	@for i in `seq 1 25`; do \
		echo -n "="; \
		sleep 0.001; \
	done
	@echo "]$(RESET)"
	@echo "$(BOLD)$(MAGENTA) -> $@ Compiled! $(RESET)"
endef

%.o: %.c
	@echo "$(MAGENTA)Compiling $<...$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@
	@$(compile_animation)

clean:
	@echo "$(RED)Cleaning object files and libraries...$(RESET)"
	@make -C $(LIBFTPRINTF_PATH) clean
	@make -C $(MLX_PATH) clean
	@$(RM) $(OBJS)
	@echo "$(GREEN)Cleaned successfully!$(RESET)"

bonus:
	make all BONUS=1
	
fclean: clean
	@echo "$(RED)Removing executable and all libraries...$(RESET)"
	@make -C $(LIBFTPRINTF_PATH) fclean
	@$(RM) $(NAME)
	@echo "$(GREEN)Full clean completed successfully!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
