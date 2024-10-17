/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_pos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 12:57:06 by mcombeau          #+#    #+#             */
/*   Updated: 2024/10/04 11:03:26 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static bool	is_valid_pos_wall_collision(t_cubi *cubi, double x, double y)
{
	if (cubi->map[(int)y][(int)x] == '0')
		return (true);
	return (false);
}

static bool	is_valid_pos_in_map(t_cubi *cubi, double x, double y)
{
	if (x < 0.25 || x >= cubi->mapinfo.width - 1.25)
		return (false);
	if (y < 0.25 || y >= cubi->mapinfo.height -0.25)
		return (false);
	return (true);
}

static bool	is_valid_pos(t_cubi *cubi, double x, double y)
{
	if (!BONUS && is_valid_pos_in_map(cubi, x, y))
		return (true);
	if (BONUS && is_valid_pos_wall_collision(cubi, x, y))
		return (true);
	return (false);
}

int	validate_move(t_cubi *cubi, double new_x, double new_y)
{
	int	moved;

	moved = 0;
	if (is_valid_pos(cubi, new_x, cubi->player.pos_y))
	{
		cubi->player.pos_x = new_x;
		moved = 1;
	}
	if (is_valid_pos(cubi, cubi->player.pos_x, new_y))
	{
		cubi->player.pos_y = new_y;
		moved = 1;
	}
	return (moved);
}
