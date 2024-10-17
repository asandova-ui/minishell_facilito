/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_things_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:56:04 by alonso            #+#    #+#             */
/*   Updated: 2024/10/07 11:01:33 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	*xpm_to_img(t_cubi *cubi, char *path)
{
	t_img	tmp;
	int		*buffer;
	int		x;
	int		y;

	init_texture_img(cubi, &tmp, path);
	buffer = ft_calloc(1,
			sizeof * buffer * cubi->texinfo.size * cubi->texinfo.size);
	if (!buffer)
		full_exit(cubi, custom_error(NULL, "error malloc", 1));
	y = 0;
	while (y < cubi->texinfo.size)
	{
		x = 0;
		while (x < cubi->texinfo.size)
		{
			buffer[y * cubi->texinfo.size + x]
				= tmp.addr[y * cubi->texinfo.size + x];
			++x;
		}
		y++;
	}
	mlx_destroy_image(cubi->mlx, tmp.img);
	return (buffer);
}

void	init_textures(t_cubi *cubi)
{
	cubi->textures = ft_calloc(5, sizeof * cubi->textures);
	if (!cubi->textures)
		full_exit(cubi, custom_error(NULL, "error malloc", 1));
	cubi->textures[NORTH] = xpm_to_img(cubi, cubi->texinfo.north);
	cubi->textures[SOUTH] = xpm_to_img(cubi, cubi->texinfo.south);
	cubi->textures[EAST] = xpm_to_img(cubi, cubi->texinfo.east);
	cubi->textures[WEST] = xpm_to_img(cubi, cubi->texinfo.west);
}

void	init_texinfo(t_texinfo *textures)
{
	textures->north = NULL;
	textures->south = NULL;
	textures->west = NULL;
	textures->east = NULL;
	textures->floor = 0;
	textures->ceiling = 0;
	textures->hex_floor = 0x0;
	textures->hex_ceiling = 0x0;
	textures->size = TEX_SIZE;
	textures->step = 0.0;
	textures->pos = 0.0;
	textures->x = 0;
	textures->y = 0;
}
