/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nosign.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:12:42 by marvin            #+#    #+#             */
/*   Updated: 2023/09/29 18:31:29 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"
#include "../../libft/libft.h"

int	print_nosign(va_list args, int fd)
{
	char			*value;
	unsigned int	n;
	int				lenght;

	n = va_arg(args, unsigned int);
	value = ft_uitoa(n);
	ft_putstr_fd(value, fd);
	lenght = ft_strlen(value);
	free((void *)value);
	return (lenght);
}
