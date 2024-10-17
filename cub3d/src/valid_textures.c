/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:49:02 by alonso            #+#    #+#             */
/*   Updated: 2024/10/04 11:13:42 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	check_valid_rgb(int *rgb)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (rgb[i] < 0 || rgb[i] > 255)
			return (custom_error_val(rgb[i], "error valor rgb (0-255)", 1));
		i++;
	}
	return (0);
}

static unsigned long	convert_rgb_to_hex(int *rgb_tab)
{
	unsigned long	result;
	int				r;
	int				g;
	int				b;

	r = rgb_tab[0];
	g = rgb_tab[1];
	b = rgb_tab[2];
	result = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
	return (result);
}

int	textures_correct(t_cubi *cubi, t_texinfo *textures)
{
	if (!textures->north || !textures->south || !textures->west
		|| !textures->east)
		return (custom_error(cubi->mapinfo.path, "Falta textura/texturas", 1));
	if (!textures->floor || !textures->ceiling)
		return (custom_error(cubi->mapinfo.path, "Falta color/colores", 1));
	if (file_correct(textures->north, false) == 1
		|| file_correct(textures->south, false) == 1
		|| file_correct(textures->west, false) == 1
		|| file_correct(textures->east, false) == 1
		|| check_valid_rgb(textures->floor) == 1
		|| check_valid_rgb(textures->ceiling) == 1)
		return (1);
	textures->hex_floor = convert_rgb_to_hex(textures->floor);
	textures->hex_ceiling = convert_rgb_to_hex(textures->ceiling);
	return (0);
}
