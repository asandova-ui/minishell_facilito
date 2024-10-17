/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 11:31:03 by mcombeau          #+#    #+#             */
/*   Updated: 2024/10/07 11:05:45 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	key_press_handler(int key, t_cubi *cubi)
{
	if (key == XK_Escape)
		quit_cub3d(cubi);
	if (key == XK_Left)
		cubi->player.rotate -= 1;
	if (key == XK_Right)
		cubi->player.rotate += 1;
	if (key == XK_w)
		cubi->player.move_y = 1;
	if (key == XK_a)
		cubi->player.move_x = -1;
	if (key == XK_s)
		cubi->player.move_y = -1;
	if (key == XK_d)
		cubi->player.move_x = 1;
	return (0);
}

static int	key_release_handler(int key, t_cubi *cubi)
{
	if (key == XK_Escape)
		quit_cub3d(cubi);
	if (key == XK_w && cubi->player.move_y == 1)
		cubi->player.move_y = 0;
	if (key == XK_s && cubi->player.move_y == -1)
		cubi->player.move_y = 0;
	if (key == XK_a && cubi->player.move_x == -1)
		cubi->player.move_x += 1;
	if (key == XK_d && cubi->player.move_x == 1)
		cubi->player.move_x -= 1;
	if (key == XK_Left && cubi->player.rotate <= 1)
		cubi->player.rotate = 0;
	if (key == XK_Right && cubi->player.rotate >= -1)
		cubi->player.rotate = 0;
	return (0);
}

static void	wrap_mouse_position(t_cubi *cubi, int x, int y)
{
	if (x > cubi->win_width - DIST_EDGE_MOUSE_WRAP)
	{
		x = DIST_EDGE_MOUSE_WRAP;
		mlx_mouse_move(cubi->mlx, cubi->win, x, y);
	}
	if (x < DIST_EDGE_MOUSE_WRAP)
	{
		x = cubi->win_width - DIST_EDGE_MOUSE_WRAP;
		mlx_mouse_move(cubi->mlx, cubi->win, x, y);
	}
}

static int	mouse_motion_handler(int x, int y, t_cubi *cubi)
{
	static int	old_x = WIN_WIDTH / 2;

	wrap_mouse_position(cubi, x, y);
	if (x == old_x)
		return (0);
	else if (x < old_x)
		cubi->player.has_moved += player_rotate(cubi, -1);
	else if (x > old_x)
		cubi->player.has_moved += player_rotate(cubi, 1);
	old_x = x;
	return (0);
}

void	listen_for_input(t_cubi *cubi)
{
	mlx_hook(cubi->win, ClientMessage, NoEventMask, quit_cub3d, cubi);
	mlx_hook(cubi->win, KeyPress, KeyPressMask, key_press_handler, cubi);
	mlx_hook(cubi->win, KeyRelease, KeyReleaseMask, key_release_handler, cubi);
	if (BONUS)
		mlx_hook(cubi->win, MotionNotify, PointerMotionMask,
			mouse_motion_handler, cubi);
}
