/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 11:30:33 by mcombeau          #+#    #+#             */
/*   Updated: 2024/10/04 12:00:28 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	init_texture_pixels(t_cubi *cubi)
{
	int	i;

	if (cubi->texture_pixels)
		free_tab((void **)cubi->texture_pixels);
	cubi->texture_pixels = ft_calloc(cubi->win_height + 1,
			sizeof * cubi->texture_pixels);
	if (!cubi->texture_pixels)
		full_exit(cubi, custom_error(NULL, "error malloc", 1));
	i = 0;
	while (i < cubi->win_height)
	{
		cubi->texture_pixels[i] = ft_calloc(cubi->win_width + 1,
				sizeof * cubi->texture_pixels);
		if (!cubi->texture_pixels[i])
			full_exit(cubi, custom_error(NULL, "error malloc", 1));
		i++;
	}
}

static void	get_texture_index(t_cubi *cubi, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->dir_x < 0)
			cubi->texinfo.index = WEST;
		else
			cubi->texinfo.index = EAST;
	}
	else
	{
		if (ray->dir_y > 0)
			cubi->texinfo.index = SOUTH;
		else
			cubi->texinfo.index = NORTH;
	}
}

void	update_texture_pixels(t_cubi *cubi, t_texinfo *tex, t_ray *ray, int x)
{
	int			y;
	int			color;

	get_texture_index(cubi, ray);
	tex->x = (int)(ray->wall_x * tex->size);
	if ((ray->side == 0 && ray->dir_x < 0)
		|| (ray->side == 1 && ray->dir_y > 0))
		tex->x = tex->size - tex->x - 1;
	tex->step = 1.0 * tex->size / ray->line_height;
	tex->pos = (ray->draw_start - cubi->win_height / 2
			+ ray->line_height / 2) * tex->step;
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		tex->y = (int)tex->pos & (tex->size - 1);
		tex->pos += tex->step;
		color = cubi->textures[tex->index][tex->size * tex->y + tex->x];
		if (tex->index == NORTH || tex->index == EAST)
			color = (color >> 1) & 8355711;
		if (color > 0)
			cubi->texture_pixels[y][x] = color;
		y++;
	}
}
