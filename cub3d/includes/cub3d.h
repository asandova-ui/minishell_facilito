/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 10:44:46 by alonso            #+#    #+#             */
/*   Updated: 2024/10/17 11:05:35 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../get_next_line/get_next_line.h"
# include "../mmllxx/mlx.h"
# include "../printf/includes/ft_printf.h"
# include "../printf/libft/libft.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
// DEFINIMOS COLORES QUE VAN A SER DE UTILIDAD

# define RESET "\e[0m"

# define BOLD "\e[1m"
# define DIM "\e[2m"
# define ITAL "\e[3m"
# define ULINE "\e[4m"

# define BLACK "\e[30m"
# define RED "\e[31m"
# define GREEN "\e[32m"
# define YELLOW "\e[33m"
# define BLUE "\e[34m"
# define PURPLE "\e[35m"
# define CYAN "\e[36m"
# define WHITE "\e[37m"

# define BRIGHT_BLACK "\e[90m"
# define BRIGHT_RED "\e[91m"
# define BRIGHT_GREEN "\e[92m"
# define BRIGHT_YELLOW "\e[93m"
# define BRIGHT_BLUE "\e[94m"
# define BRIGHT_PURPLE "\e[95m"
# define BRIGHT_CYAN "\e[96m"
# define BRIGHT_WHITE "\e[97m"

# define BG_BLACK "\e[40m"
# define BG_RED "\e[41m"
# define BG_GREEN "\e[42m"
# define BG_YELLOW "\e[43m"
# define BG_BLUE "\e[44m"
# define BG_PURPLE "\e[45m"
# define BG_CYAN "\e[46m"
# define BG_WHITE "\e[47m"

# define BG_BRIGHT_BLACK "\e[100m"
# define BG_BRIGHT_RED "\e[101m"
# define BG_BRIGHT_GREEN "\e[102m"
# define BG_BRIGHT_YELLOW "\e[103m"
# define BG_BRIGHT_BLUE "\e[104m"
# define BG_BRIGHT_PURPLE "\e[105m"
# define BG_BRIGHT_CYAN "\e[106m"
# define BG_BRIGHT_WHITE "\e[107m"

// PARAMETERS MODIFICABLES
# define WIN_WIDTH 1280
# define WIN_HEIGHT 960
# define TEX_SIZE 64

# ifndef BONUS
#  define BONUS 0
# endif

# ifndef O_DIRECTORY
#  define O_DIRECTORY 00200000
# endif

# define NORTH 0
# define SOUTH 1
# define EAST 2
# define WEST 3

# define MOVEMENTSPEED 0.12
# define ROTATIONSPEED 0.06

# define DIST_EDGE_MOUSE_WRAP 20

/* MINIMAP MACROS */
# define MMAP_PIXEL_SIZE 128
# define MMAP_VIEW_DIST 4
# define MMAP_COLOR_PLAYER 0x00FF00
# define MMAP_COLOR_WALL 0x808080
# define MMAP_COLOR_FLOOR 0xE6E6E6
# define MMAP_COLOR_SPACE 0x404040

typedef struct s_img
{
	void			*img;
	int				*addr;
	int				pixel_bits;
	int				size_line;
	int				endian;
}					t_img;

typedef struct s_texinfo
{
	char			*north;
	char			*south;
	char			*west;
	char			*east;
	int				*floor;
	int				*ceiling;
	unsigned long	hex_floor;
	unsigned long	hex_ceiling;
	int				size;
	int				index;
	double			step;
	double			pos;
	int				x;
	int				y;
}					t_texinfo;

typedef struct s_mapinfo
{
	int				fd;
	int				line_count;
	char			*path;
	char			**file;
	int				height;
	int				width;
	int				index_end_of_map;
}					t_mapinfo;

typedef struct s_ray
{
	double			camera_x;
	double			dir_x;
	double			dir_y;
	int				map_x;
	int				map_y;
	int				step_x;
	int				step_y;
	double			sidedist_x;
	double			sidedist_y;
	double			deltadist_x;
	double			deltadist_y;
	double			wall_dist;
	double			wall_x;
	int				side;
	int				line_height;
	int				draw_start;
	int				draw_end;
}					t_ray;

typedef struct s_player
{
	char			dir;
	double			pos_x;
	double			pos_y;
	double			dir_x;
	double			dir_y;
	double			plane_x;
	double			plane_y;
	int				has_moved;
	int				move_x;
	int				move_y;
	int				rotate;
}					t_player;

typedef struct s_cubi
{
	void			*mlx;
	void			*win;
	int				win_height;
	int				win_width;
	t_mapinfo		mapinfo;
	char			**map;
	t_player		player;
	t_ray			ray;
	int				**texture_pixels;
	int				**textures;
	t_texinfo		texinfo;
	t_img			minimap;
}					t_cubi;

typedef struct s_minimap
{
	char			**map;
	t_img			*img;
	int				size;
	int				offset_x;
	int				offset_y;
	int				view_dist;
	int				tile_size;
}					t_minimap;

int					custom_error(char *detail, char *str, int code);
void				init_cubi(t_cubi *cubi);
int					file_correct(char *arg, bool cub);
void				full_exit(t_cubi *cubi, int code);
void				parse_cubi(char *path, t_cubi *cubi);
int					get_cubi_file(t_cubi *cubi, char **map);
void				free_tab(void **tab);
int					fill_color_textures(t_cubi *cubi, t_texinfo *textures,
						char *line, int j);
void				init_mlx(t_cubi *cubi);
int					cubi_freeall(t_cubi *cubi);
int					custom_error_val(int detail, char *str, int code);
int					textures_correct(t_cubi *cubi, t_texinfo *textures);
void				dir_and_ply(t_cubi *cubi);
void				debug_display_data(t_cubi *cubi);
int					map_correct(t_cubi *cubi, char **map_tab);
void				init_img_clean(t_img *img);
int					create_map(t_cubi *cubi, char **file, int i);
int					is_a_white_space(char c);
size_t				find_biggest_len(t_mapinfo *map, int i);
void				init_textures(t_cubi *cubi);
void				init_texture_img(t_cubi *cubi, t_img *image, char *path);
int					render(t_cubi *cubi);
void				render_images(t_cubi *cubi);
void				set_image_pixel(t_img *image, int x, int y, int color);
void				init_img(t_cubi *cubi, t_img *image, int width, int height);
void				init_texture_pixels(t_cubi *cubi);
void				init_ray(t_ray *ray);
int					raycasting(t_player *player, t_cubi *cubi);
void				render_minimap_image(t_cubi *cubi, t_minimap *minimap);
void				render_minimap(t_cubi *cubi);
int					move_player(t_cubi *cubi);
void				debug_display_minimap(t_minimap *minimap);
void				update_texture_pixels(t_cubi *cubi, t_texinfo *tex,
						t_ray *ray, int x);
int					player_rotate(t_cubi *cubi, double rotdir);
int					quit_cub3d(t_cubi *cubi);
int					validate_move(t_cubi *cubi, double new_x, double new_y);
void				listen_for_input(t_cubi *cubi);
void				init_texinfo(t_texinfo *textures);
int					check_player_position(t_cubi *cubi, char **map_tab);
int					check_map_elements(t_cubi *cubi, char **map_tab);
int					check_map_sides(t_mapinfo *map, char **map_tab);
int					check_top_or_bottom(char **map_tab, int i, int j);

#endif