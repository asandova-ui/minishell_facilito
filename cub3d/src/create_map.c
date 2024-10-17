/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:08:20 by alonso            #+#    #+#             */
/*   Updated: 2024/10/04 12:16:42 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	count_map_lines(t_cubi *cubi, char **file, int i)
{
	int	index_value;
	int	j;

	index_value = i;
	while (file[i])
	{
		j = 0;
		while (file[i][j] == ' ' || file[i][j] == '\t' || file[i][j] == '\r'
			|| file[i][j] == '\v' || file[i][j] == '\f')
			j++;
		if (file[i][j] != '1')
			break ;
		i++;
	}
	cubi->mapinfo.index_end_of_map = i;
	return (i - index_value);
}

static int	fill_map_tab(t_mapinfo *mapinfo, char **map_tab, int index)
{
	int	i;
	int	j;

	mapinfo->width = find_biggest_len(mapinfo, index);
	i = 0;
	while (i < mapinfo->height)
	{
		j = 0;
		map_tab[i] = malloc(sizeof(char) * (mapinfo->width + 1));
		if (!map_tab[i])
			return (custom_error(NULL, "error malloc", 1));
		while (mapinfo->file[index][j] && mapinfo->file[index][j] != '\n')
		{
			map_tab[i][j] = mapinfo->file[index][j];
			j++;
		}
		while (j < mapinfo->width)
			map_tab[i][j++] = '\0';
		i++;
		index++;
	}
	map_tab[i] = NULL;
	return (0);
}

static int	get_map_info(t_cubi *cubi, char **file, int i)
{
	cubi->mapinfo.height = count_map_lines(cubi, file, i);
	cubi->map = malloc(sizeof(char *) * (cubi->mapinfo.height + 1));
	if (!cubi->map)
		return (custom_error(NULL, "error malloc", 1));
	if (fill_map_tab(&cubi->mapinfo, cubi->map, i) == 1)
		return (1);
	return (0);
}

static void	change_space_into_wall(t_cubi *cubi)
{
	int	i;
	int	j;

	i = 0;
	while (cubi->map[i])
	{
		j = 0;
		while (cubi->map[i][j] == ' ' || cubi->map[i][j] == '\t'
			|| cubi->map[i][j] == '\r' || cubi->map[i][j] == '\v'
			|| cubi->map[i][j] == '\f')
			j++;
		while (cubi->map[i][++j])
		{
			if (cubi->map[i][j] == ' '
				&& j != cubi->map[i][ft_strlen(cubi->map[i]) - 1])
				cubi->map[i][j] = '1';
		}
		i++;
	}
}

int	create_map(t_cubi *cubi, char **file, int i)
{
	if (get_map_info(cubi, file, i) == 1)
		return (1);
	change_space_into_wall(cubi);
	return (0);
}
