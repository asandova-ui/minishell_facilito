/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_character.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 16:46:42 by marvin            #+#    #+#             */
/*   Updated: 2023/09/29 18:31:17 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"
#include "../../libft/libft.h"

int	print_character(va_list args, int fd)
{
	char	value;
	int		length;

	value = va_arg(args, int);
	ft_putchar_fd(value, fd);
	length = 1;
	return (length);
}
