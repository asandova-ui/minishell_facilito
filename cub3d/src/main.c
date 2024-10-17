/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 10:44:27 by alonso            #+#    #+#             */
/*   Updated: 2024/10/07 11:01:34 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	parse_args(t_cubi *cubi, char **argv)
{
	if (file_correct(argv[1], true) == 1)
		full_exit(cubi, 1);
	parse_cubi(argv[1], cubi);
	if (get_cubi_file(cubi, cubi->mapinfo.file) == 1)
		return (cubi_freeall(cubi));
	if (map_correct(cubi, cubi->map) == 1)
		return (cubi_freeall(cubi));
	if (textures_correct(cubi, &cubi->texinfo) == 1)
		return (cubi_freeall(cubi));
	dir_and_ply(cubi);
	return (0);
}

void	wait_for_mlx(t_cubi *cubi)
{
	mlx_loop_hook(cubi->mlx, render, cubi);
	mlx_loop(cubi->mlx);
}

int	cub3d(t_cubi *cubi, char **argv)
{
	if (parse_args(cubi, argv) != 0)
		return (1);
	init_mlx(cubi);
	init_textures(cubi);
	render_images(cubi);
	listen_for_input(cubi);
	wait_for_mlx(cubi);
	return (0);
}

int	main(int argc, char **argv)
{
	t_cubi	cubi;

	if (argc != 2)
		return (custom_error("Numero incorrecto de parametros", "dale bien",
				1));
	init_cubi(&cubi);
	if (cub3d(&cubi, argv) == 1)
		return (1);
	return (0);
}
