/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_things.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:30:19 by alonso            #+#    #+#             */
/*   Updated: 2024/10/04 11:14:24 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	free_tab(void **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	if (tab)
	{
		free(tab);
		tab = NULL;
	}
}

static void	free_texinfo(t_texinfo *textures)
{
	if (textures->north)
		free(textures->north);
	if (textures->south)
		free(textures->south);
	if (textures->west)
		free(textures->west);
	if (textures->east)
		free(textures->east);
	if (textures->floor)
		free(textures->floor);
	if (textures->ceiling)
		free(textures->ceiling);
}

static void	free_map(t_cubi *cubi)
{
	if (cubi->mapinfo.fd > 0)
		close(cubi->mapinfo.fd);
	if (cubi->mapinfo.file)
		free_tab((void **)cubi->mapinfo.file);
	if (cubi->map)
		free_tab((void **)cubi->map);
}

int	cubi_freeall(t_cubi *cubi)
{
	if (cubi->textures)
		free_tab((void **)cubi->textures);
	if (cubi->texture_pixels)
		free_tab((void **)cubi->texture_pixels);
	free_texinfo(&cubi->texinfo);
	free_map(cubi);
	return (1);
}
