/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 11:30:04 by mcombeau          #+#    #+#             */
/*   Updated: 2024/10/04 11:03:33 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	set_frame_image_pixel(t_cubi *cubi, t_img *image, int x, int y)
{
	if (cubi->texture_pixels[y][x] > 0)
		set_image_pixel(image, x, y, cubi->texture_pixels[y][x]);
	else if (y < cubi->win_height / 2)
		set_image_pixel(image, x, y, cubi->texinfo.hex_ceiling);
	else if (y < cubi->win_height -1)
		set_image_pixel(image, x, y, cubi->texinfo.hex_floor);
}

static void	render_frame(t_cubi *cubi)
{
	t_img	image;
	int		x;
	int		y;

	image.img = NULL;
	init_img(cubi, &image, cubi->win_width, cubi->win_height);
	y = 0;
	while (y < cubi->win_height)
	{
		x = 0;
		while (x < cubi->win_width)
		{
			set_frame_image_pixel(cubi, &image, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(cubi->mlx, cubi->win, image.img, 0, 0);
	mlx_destroy_image(cubi->mlx, image.img);
}

static void	render_raycast(t_cubi *cubi)
{
	init_texture_pixels(cubi);
	init_ray(&cubi->ray);
	raycasting(&cubi->player, cubi);
	render_frame(cubi);
}

void	render_images(t_cubi *cubi)
{
	render_raycast(cubi);
	if (BONUS)
		render_minimap(cubi);
}

int	render(t_cubi *cubi)
{
	cubi->player.has_moved += move_player(cubi);
	if (cubi->player.has_moved == 0)
		return (0);
	render_images(cubi);
	return (0);
}
