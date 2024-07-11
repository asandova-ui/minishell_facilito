/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 11:24:27 by asandova          #+#    #+#             */
/*   Updated: 2023/09/29 18:04:26 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	char	*temp;

	temp = s;
	while (*temp)
	{
		ft_putchar_fd(*temp, fd);
		temp++;
	}
}
