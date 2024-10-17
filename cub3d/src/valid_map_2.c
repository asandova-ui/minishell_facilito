/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:22:34 by alonso            #+#    #+#             */
/*   Updated: 2024/10/04 12:22:51 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	check_map_is_at_the_end(t_mapinfo *map)
{
	int	i;
	int	j;

	i = map->index_end_of_map;
	while (map->file[i])
	{
		j = 0;
		while (map->file[i][j])
		{
			if (map->file[i][j] != ' ' && map->file[i][j] != '\t'
				&& map->file[i][j] != '\r' && map->file[i][j] != '\n'
				&& map->file[i][j] != '\v' && map->file[i][j] != '\f')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	map_correct(t_cubi *cubi, char **map_tab)
{
	if (!cubi->map)
		return (custom_error(cubi->mapinfo.path, "No hay mapa", 1));
	if (check_map_sides(&cubi->mapinfo, map_tab) == 1)
		return (custom_error(cubi->mapinfo.path, "Mapa no rodeado de 1", 1));
	if (cubi->mapinfo.height < 3)
		return (custom_error(cubi->mapinfo.path, "Mapa pequeño", 1));
	if (check_map_elements(cubi, map_tab) == 1)
		return (1);
	if (check_player_position(cubi, map_tab) == 1)
		return (1);
	if (check_map_is_at_the_end(&cubi->mapinfo) == 1)
		return (custom_error(cubi->mapinfo.path, "Mapa no esta al final", 1));
	return (0);
}
