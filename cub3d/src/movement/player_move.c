/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 11:31:24 by mcombeau          #+#    #+#             */
/*   Updated: 2024/10/07 11:05:45 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	move_player_forward(t_cubi *cubi)
{
	double	new_x;
	double	new_y;

	new_x = cubi->player.pos_x + cubi->player.dir_x * MOVEMENTSPEED;
	new_y = cubi->player.pos_y + cubi->player.dir_y * MOVEMENTSPEED;
	return (validate_move(cubi, new_x, new_y));
}

static int	move_player_backward(t_cubi *cubi)
{
	double	new_x;
	double	new_y;

	new_x = cubi->player.pos_x - cubi->player.dir_x * MOVEMENTSPEED;
	new_y = cubi->player.pos_y - cubi->player.dir_y * MOVEMENTSPEED;
	return (validate_move(cubi, new_x, new_y));
}

static int	move_player_left(t_cubi *cubi)
{
	double	new_x;
	double	new_y;

	new_x = cubi->player.pos_x + cubi->player.dir_y * MOVEMENTSPEED;
	new_y = cubi->player.pos_y - cubi->player.dir_x * MOVEMENTSPEED;
	return (validate_move(cubi, new_x, new_y));
}

static int	move_player_right(t_cubi *cubi)
{
	double	new_x;
	double	new_y;

	new_x = cubi->player.pos_x - cubi->player.dir_y * MOVEMENTSPEED;
	new_y = cubi->player.pos_y + cubi->player.dir_x * MOVEMENTSPEED;
	return (validate_move(cubi, new_x, new_y));
}

int	move_player(t_cubi *cubi)
{
	int	moved;

	moved = 0;
	if (cubi->player.move_y == 1)
		moved += move_player_forward(cubi);
	if (cubi->player.move_y == -1)
		moved += move_player_backward(cubi);
	if (cubi->player.move_x == -1)
		moved += move_player_left(cubi);
	if (cubi->player.move_x == 1)
		moved += move_player_right(cubi);
	if (cubi->player.rotate != 0)
		moved += player_rotate(cubi, cubi->player.rotate);
	return (moved);
}
