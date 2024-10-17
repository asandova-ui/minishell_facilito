/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:41:27 by alonso            #+#    #+#             */
/*   Updated: 2024/10/04 12:26:49 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	check_top_or_bottom(char **map_tab, int i, int j)
{
	if (!map_tab || !map_tab[i] || !map_tab[i][j])
		return (1);
	while (map_tab[i][j] == ' ' || map_tab[i][j] == '\t'
	|| map_tab[i][j] == '\r' || map_tab[i][j] == '\v'
	|| map_tab[i][j] == '\f')
		j++;
	while (map_tab[i][j])
	{
		if (map_tab[i][j] != '1')
			return (1);
		j++;
	}
	return (0);
}

int	check_map_sides(t_mapinfo *map, char **map_tab)
{
	int	i;
	int	j;

	if (check_top_or_bottom(map_tab, 0, 0) == 1)
		return (1);
	i = 1;
	while (i < (map->height - 1))
	{
		j = ft_strlen(map_tab[i]) - 1;
		if (map_tab[i][j] != '1')
			return (1);
		i++;
	}
	if (check_top_or_bottom(map_tab, i, 0) == 1)
		return (1);
	return (0);
}

int	check_map_elements(t_cubi *cubi, char **map_tab)
{
	int	i;
	int	j;

	i = 0;
	cubi->player.dir = '0';
	while (map_tab[i] != NULL)
	{
		j = 0;
		while (map_tab[i][j])
		{
			while (cubi->map[i][j] == ' ' || cubi->map[i][j] == '\t'
			|| cubi->map[i][j] == '\r'
			|| cubi->map[i][j] == '\v' || cubi->map[i][j] == '\f')
				j++;
			if (!(ft_strchr("10NSEW", map_tab[i][j])))
				return (custom_error(cubi->mapinfo.path, "Char invalido", 1));
			if (ft_strchr("NSEW", map_tab[i][j]) && cubi->player.dir != '0')
				return (custom_error(cubi->mapinfo.path, "Solo 1 player", 1));
			if (ft_strchr("NSEW", map_tab[i][j]) && cubi->player.dir == '0')
				cubi->player.dir = map_tab[i][j];
			j++;
		}
		i++;
	}
	return (0);
}

int	check_position_is_valid(t_cubi *cubi, char **map_tab)
{
	int	i;
	int	j;

	i = (int)cubi->player.pos_y;
	j = (int)cubi->player.pos_x;
	if (ft_strlen(map_tab[i - 1]) < (size_t)j
		|| ft_strlen(map_tab[i + 1]) < (size_t)j
		|| is_a_white_space(map_tab[i][j - 1]) == 0
		|| is_a_white_space(map_tab[i][j + 1]) == 0
		|| is_a_white_space(map_tab[i - 1][j]) == 0
		|| is_a_white_space(map_tab[i + 1][j]) == 0)
		return (1);
	return (0);
}

int	check_player_position(t_cubi *cubi, char **map_tab)
{
	int	i;
	int	j;

	if (cubi->player.dir == '0')
		return (custom_error(cubi->mapinfo.path, "Falta direccion", 1));
	i = 0;
	while (map_tab[i])
	{
		j = 0;
		while (map_tab[i][j])
		{
			if (ft_strchr("NSEW", map_tab[i][j]))
			{
				cubi->player.pos_x = (double)j + 0.5;
				cubi->player.pos_y = (double)i + 0.5;
				map_tab[i][j] = '0';
			}
			j++;
		}
		i++;
	}
	if (check_position_is_valid(cubi, map_tab) == 1)
		return (custom_error(cubi->mapinfo.path, "Posicion invalida", 1));
	return (0);
}
