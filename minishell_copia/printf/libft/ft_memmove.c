/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:43:53 by asandova            #+#    #+#           */
/*   Updated: 2023/09/18 17:43:53 by asandova           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	int					i;

	if (!dst && !src)
		return (0);
	if (dst > src)
	{
		i = (int)n;
		while (--i >= 0)
			*(char *)(dst + i) = *(char *)(src + i);
	}
	else
	{
		i = -1;
		while (++i < (int)n)
			*(char *)(dst + i) = *(char *)(src + i);
	}
	return (dst);
}
