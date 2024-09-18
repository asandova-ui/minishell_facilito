/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 17:09:56 by marvin            #+#    #+#             */
/*   Updated: 2023/11/03 10:10:01 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <limits.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

int		print_integer(va_list args, int fd);
int		print_nosign(va_list args, int fd);
int		print_highhex(va_list args, int fd);
int		print_lowhex(va_list args, int fd);
int		print_character(va_list args, int fd);
int		print_string(va_list args, int fd);
int		print_ptrhexa(va_list args, int fd);

char	*ft_uitoa(unsigned int n);
char	*ft_itoa_base(unsigned int n, char *base);

int		ft_printf(const char *format, ...);
int		format_manager(const char *ptr, va_list args);
void	ft_putchar_fd(char c, int fd);
#endif