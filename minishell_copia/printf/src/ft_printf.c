/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:43:48 by asandova          #+#    #+#             */
/*   Updated: 2023/11/03 10:10:01 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"
#include "../libft/libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}

int	ft_printf(const char *format, ...)
{
	int			chars_written;
	va_list		args;
	const char	*ptr;

	va_start(args, format);
	ptr = format;
	chars_written = 0;
	while (*ptr != '\0')
	{
		if (*ptr == '%')
		{
			ptr++;
			chars_written += format_manager(ptr, args);
		}
		else
		{
			ft_putchar_fd(*ptr, 1);
			chars_written++;
		}
		ptr++;
	}
	va_end(args);
	return (chars_written);
}

int	format_manager(const char *ptr, va_list args)
{
	if (*ptr == 'd' || *ptr == 'i')
		return (print_integer(args, 1));
	else if (*ptr == 'u')
		return (print_nosign(args, 1));
	else if (*ptr == 'x')
		return (print_lowhex(args, 1));
	else if (*ptr == 'X')
		return (print_highhex(args, 1));
	else if (*ptr == 'c')
		return (print_character(args, 1));
	else if (*ptr == 's')
		return (print_string(args, 1));
	else if (*ptr == 'p')
		return (print_ptrhexa(args, 1));
	else
	{
		ft_putchar_fd('%', 1);
		return (1);
	}
}
