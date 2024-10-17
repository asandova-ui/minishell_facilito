/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:36:17 by alonso            #+#    #+#             */
/*   Updated: 2024/10/07 11:00:35 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_img(t_cubi *cubi, t_img *image, int width, int height)
{
	init_img_clean(image);
	image->img = mlx_new_image(cubi->mlx, width, height);
	if (image->img == NULL)
		full_exit(cubi, custom_error("mlx", "Error creando la imagen mlx", 1));
	image->addr = (int *)mlx_get_data_addr(image->img, &image->pixel_bits,
			&image->size_line, &image->endian);
	return ;
}

void	init_texture_img(t_cubi *cubi, t_img *image, char *path)
{
	init_img_clean(image);
	image->img = mlx_xpm_file_to_image(cubi->mlx, path, &cubi->texinfo.size,
			&cubi->texinfo.size);
	if (image->img == NULL)
		full_exit(cubi, custom_error("mlx", "Error creando la imagen mlx", 1));
	image->addr = (int *)mlx_get_data_addr(image->img, &image->pixel_bits,
			&image->size_line, &image->endian);
	return ;
}

void	init_mlx(t_cubi *cubi)
{
	cubi->mlx = mlx_init();
	if (!cubi->mlx)
		full_exit(cubi, custom_error("mlx", "Error para arrancar mlx", 1));
	cubi->win = mlx_new_window(cubi->mlx, WIN_WIDTH, WIN_HEIGHT, "Cub3D");
	if (!cubi->win)
		full_exit(cubi, custom_error("mlx", "Error creando la ventana mlx", 1));
	if (BONUS)
		mlx_mouse_move(cubi->mlx, cubi->win, cubi->win_width / 2,
			cubi->win_height / 2);
	return ;
}
