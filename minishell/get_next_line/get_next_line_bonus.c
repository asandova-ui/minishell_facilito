/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 10:50:19 by marvin            #+#    #+#             */
/*   Updated: 2023/10/04 10:50:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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

	ptr = ft_strchr(fd_storage->storage, '\n');
	if (!ptr)
	{
		temp_st = NULL;
		return (remix_free(&fd_storage->storage));
	}
	else
		lo = ptr - fd_storage->storage + 1;
	if (!fd_storage->storage[lo])
		return (remix_free(&fd_storage->storage));
	fd_storage->length = ft_strlen(fd_storage->storage);
	temp_st = ft_substr(fd_storage->storage, lo, fd_storage->length - lo);
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

	sto = ft_strchr(fd_storage->storage, '\n');
	if (!sto)
		lo = (int)ft_strlen(fd_storage->storage);
	else
		lo = sto - fd_storage->storage + 1;
	returning_line = ft_substr(fd_storage->storage, 0, lo);
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
	while (lo_bytes > 0 && !ft_strchr(fd_storage->storage, '\n'))
	{
		lo_bytes = read(fd, buff, BUFFER_SIZE);
		if (lo_bytes > 0)
		{
			buff[lo_bytes] = '\0';
			fd_storage->storage = ft_strjoin(fd_storage->storage, buff);
		}
	}
	free(buff);
	if (lo_bytes == -1)
		return (remix_free(&fd_storage->storage));
	return (fd_storage->storage);
}

char	*get_next_line(int fd)
{
	static t_fd_storage	fd_storages[MAX_FD];
	char				*line;

	if (fd < 0 || fd >= MAX_FD)
		return (NULL);
	if (!fd_storages[fd].storage || !ft_strchr(fd_storages[fd].storage, '\n'))
		fd_storages[fd].storage = base_file_reading(fd, &fd_storages[fd]);
	if (!fd_storages[fd].storage)
		return (NULL);
	line = line_getting(&fd_storages[fd]);
	if (!line)
		return (remix_free(&fd_storages[fd].storage));
	fd_storages[fd].storage = line_keep(&fd_storages[fd]);
	if (!ft_strchr(line, '\n'))
	{
		line[ft_strlen(line)] = '\0';
		return (line);
	}
	return (line);
}
