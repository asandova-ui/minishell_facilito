/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptrhexa.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:27:27 by marvin            #+#    #+#             */
/*   Updated: 2024/05/05 21:38:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"
#include "../../libft/libft.h"

static void	ft_putnbr_hex(intptr_t n, int fd)
{
	const char	*hex_chars = "0123456789abcdef";

	if (n >= 16)
		ft_putnbr_hex(n / 16, fd);
	ft_putchar_fd(hex_chars[n % 16], fd);
}

int	print_ptrhexa(va_list args, int fd)
{
	intptr_t	ptr_value;
	int			chars_written;

	ptr_value = va_arg(args, intptr_t);
	chars_written = 0;
	ft_putchar_fd('0', fd);
	ft_putchar_fd('x', fd);
	chars_written += 2;
	if (!ptr_value)
	{
		ft_putchar_fd('0', fd);
		chars_written ++;
		return (chars_written);
	}
	ft_putnbr_hex(ptr_value, fd);
	while (ptr_value != 0)
	{
		ptr_value /= 16;
		chars_written++;
	}
	return (chars_written);
}
