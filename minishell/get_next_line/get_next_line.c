/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 11:43:54 by marvin            #+#    #+#             */
/*   Updated: 2024/07/08 16:03:09 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*remix_free(char **storage)
{
	free(*storage);
	*storage = NULL;
	return (NULL);
}

char	*line_keep(t_fd_storage *fd_storage)
{
	char	*temp_st;
	char	*ptr;
	int		lo;

	ptr = ft_strchr2(fd_storage->storage, '\n');
	if (!ptr)
	{
		temp_st = NULL;
		return (remix_free(&fd_storage->storage));
	}
	else
		lo = ptr - fd_storage->storage + 1;
	if (!fd_storage->storage[lo])
		return (remix_free(&fd_storage->storage));
	fd_storage->length = ft_strlen2(fd_storage->storage);
	temp_st = ft_substr2(fd_storage->storage, lo, fd_storage->length - lo);
	remix_free(&fd_storage->storage);
	if (!temp_st)
		return (NULL);
	return (temp_st);
}

char	*line_getting(t_fd_storage *fd_storage)
{
	char	*returning_line;
	char	*sto;
	int		lo;

	sto = ft_strchr2(fd_storage->storage, '\n');
	if (!sto)
		lo = (int)ft_strlen2(fd_storage->storage);
	else
		lo = sto - fd_storage->storage + 1;
	returning_line = ft_substr2(fd_storage->storage, 0, lo);
	if (!returning_line)
		return (NULL);
	return (returning_line);
}

char	*base_file_reading(int fd, t_fd_storage *fd_storage)
{
	int		lo_bytes;
	char	*buff;

	lo_bytes = 1;
	buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (remix_free(&fd_storage->storage));
	buff[0] = '\0';
	while (lo_bytes > 0 && !ft_strchr2(fd_storage->storage, '\n'))
	{
		lo_bytes = read(fd, buff, BUFFER_SIZE);
		if (lo_bytes > 0)
		{
			buff[lo_bytes] = '\0';
			fd_storage->storage = ft_strjoin2(fd_storage->storage, buff);
		}
	}
	free(buff);
	if (lo_bytes == -1)
		return (remix_free(&fd_storage->storage));
	return (fd_storage->storage);
}

char	*get_next_line(int fd)
{
	static t_fd_storage	fd_storage;
	char				*line;

	if (fd < 0)
		return (NULL);
	if (!fd_storage.storage || !ft_strchr2(fd_storage.storage, '\n'))
		fd_storage.storage = base_file_reading(fd, &fd_storage);
	if (!fd_storage.storage)
		return (NULL);
	line = line_getting(&fd_storage);
	if (!line)
		return (remix_free(&fd_storage.storage));
	fd_storage.storage = line_keep(&fd_storage);
	if (!ft_strchr2(line, '\n'))
	{
		line[ft_strlen2(line)] = '\0';
		return (line);
	}
	return (line);
}
