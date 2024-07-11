/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:57:33 by asandova            #+#    #+#           */
/*   Updated: 2023/09/18 17:57:33 by asandova           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;
	char	*r;

	c = (char)c;
	r = (char *)s;
	len = ft_strlen(s);
	while (len > 0)
	{
		if (r[len] == c)
			return (&r[len]);
		len--;
	}
	if (r[len] == c)
		return (r);
	return (0);
}
