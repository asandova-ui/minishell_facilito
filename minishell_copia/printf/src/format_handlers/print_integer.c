/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_integer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:01:25 by marvin            #+#    #+#             */
/*   Updated: 2023/09/29 18:31:22 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"
#include "../../libft/libft.h"

int	print_integer(va_list args, int fd)
{
	char	*value;
	int		n;
	int		length;

	n = va_arg(args, int);
	value = ft_itoa(n);
	ft_putstr_fd(value, fd);
	length = ft_strlen(value);
	free(value);
	return (length);
}
