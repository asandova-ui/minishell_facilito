/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_highhex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:38:32 by marvin            #+#    #+#             */
/*   Updated: 2023/09/29 18:31:24 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"
#include "../../libft/libft.h"

int	print_highhex(va_list args, int fd)
{
	char	*value;
	int		n;
	int		length;

	n = va_arg(args, unsigned int);
	value = ft_itoa_base(n, "0123456789ABCDEF");
	ft_putstr_fd(value, fd);
	length = ft_strlen(value);
	free((void *)value);
	return (length);
}
