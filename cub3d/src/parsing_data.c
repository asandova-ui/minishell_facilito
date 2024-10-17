/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 12:14:01 by alonso            #+#    #+#             */
/*   Updated: 2024/10/04 12:04:31 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	get_number_of_lines(char *path)
{
	int		fd;
	char	*line;
	int		line_count;

	line_count = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		custom_error(path, strerror(errno), errno);
	else
	{
		line = get_next_line(fd);
		while (line != NULL)
		{
			line_count++;
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
	}
	return (line_count);
}

static void	fill_tab(int row, int column, int i, t_cubi *cubi)
{
	char	*line;

	line = get_next_line(cubi->mapinfo.fd);
	while (line != NULL)
	{
		cubi->mapinfo.file[row] = ft_calloc(ft_strlen(line) + 1, sizeof(char));
		if (!cubi->mapinfo.file[row])
		{
			custom_error(NULL, "eror malloc", 0);
			return (free_tab((void **)cubi->mapinfo.file));
		}
		while (line[i] != '\0')
			cubi->mapinfo.file[row][column++] = line[i++];
		cubi->mapinfo.file[row++][column] = '\0';
		column = 0;
		i = 0;
		free(line);
		line = get_next_line(cubi->mapinfo.fd);
	}
	cubi->mapinfo.file[row] = NULL;
}

void	parse_cubi(char *path, t_cubi *cubi)
{
	int		row;
	int		i;
	size_t	column;

	i = 0;
	row = 0;
	column = 0;
	cubi->mapinfo.line_count = get_number_of_lines(path);
	cubi->mapinfo.path = path;
	cubi->mapinfo.file = ft_calloc(cubi->mapinfo.line_count \
			+ 1, sizeof(char *));
	if (!(cubi->mapinfo.file))
	{
		custom_error(NULL, "error malloc", 0);
		return ;
	}
	cubi->mapinfo.fd = open(path, O_RDONLY);
	if (cubi->mapinfo.fd < 0)
		custom_error(path, strerror(errno), 1);
	else
	{
		fill_tab(row, column, i, cubi);
		close(cubi->mapinfo.fd);
	}
}
