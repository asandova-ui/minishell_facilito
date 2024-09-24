/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 16:57:52 by marvin            #+#    #+#             */
/*   Updated: 2023/10/02 17:07:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"
#include "../../libft/libft.h"

int	print_string(va_list args, int fd)
{
	char	*value;
	int		length;

	value = va_arg(args, char *);
	if (value == NULL)
	{
		ft_putstr_fd("(null)", fd);
		length = 6;
		return (length);
	}
	ft_putstr_fd(value, fd);
	length = ft_strlen(value);
	return (length);
}
