/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 11:25:20 by asandova            #+#    #+#           */
/*   Updated: 2023/09/22 11:25:20 by asandova           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	char	*temp;

	temp = s;
	while (*temp)
	{
		ft_putchar_fd(*temp, fd);
		temp++;
	}
	ft_putchar_fd('\n', fd);
}
