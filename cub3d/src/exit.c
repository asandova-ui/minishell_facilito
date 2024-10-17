/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 11:20:11 by alonso            #+#    #+#             */
/*   Updated: 2024/10/04 11:14:24 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	full_exit(t_cubi *cubi, int code)
{
	if (!cubi)
		exit(code);
	if (cubi->win && cubi->mlx)
		mlx_destroy_window(cubi->mlx, cubi->win);
	if (cubi->mlx)
	{
		mlx_destroy_display(cubi->mlx);
		mlx_loop_end(cubi->mlx);
		free(cubi->mlx);
	}
	cubi_freeall(cubi);
	exit(code);
}

int	quit_cub3d(t_cubi *cubi)
{
	full_exit(cubi, 0);
	return (0);
}
