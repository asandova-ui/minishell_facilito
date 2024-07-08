/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 10:50:16 by marvin            #+#    #+#             */
/*   Updated: 2023/10/04 10:50:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# ifndef MAX_FD
#  define MAX_FD 23
# endif

# include <fcntl.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_fd_storage
{
	char	*storage;
	size_t	length;
}			t_fd_storage;

char		*remix_free(char **str);
char		*line_keep(t_fd_storage *fd_storage);
char		*line_getting(t_fd_storage *fd_storage);
char		*base_file_reading(int fd, t_fd_storage *fd_storage);
char		*get_next_line(int fd);

char		*ft_strjoin(char *s1, char *s2);
size_t		ft_strlen(char *str);
char		*ft_substr(char *str, unsigned int start, size_t len);
char		*ft_strchr(char *str, int c);
char		*ft_strdup(char *str);
#endif