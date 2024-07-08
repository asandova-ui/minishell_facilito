/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 11:44:28 by marvin            #+#    #+#             */
/*   Updated: 2024/07/08 16:17:33 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
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

char		*ft_strjoin2(char *s1, char *s2);
size_t		ft_strlen2(char *str);
char		*ft_substr2(char *str, unsigned int start, size_t len);
char		*ft_strchr2(char *str, int c);
char		*ft_strdup2(char *str);
#endif