/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:56:38 by asandova            #+#    #+#           */
/*   Updated: 2023/09/18 17:56:38 by asandova           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

void	*ft_memchr(void *s, int c, size_t n)
{
	unsigned char	*p;
	size_t			i;

	p = (unsigned char *)s;
	i = 0;
	while (n--)
	{
		if (p[i] == (unsigned char)c)
			return ((unsigned char *)&p[i]);
		i++;
	}
	return (NULL);
}
