/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:58:49 by asandova            #+#    #+#           */
/*   Updated: 2023/09/18 17:58:49 by asandova           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	c = (char)c;
	i = 0;
	while (s[i] != c)
	{
		if (!s[i])
			return (NULL);
		i++;
	}
	return ((char *)&s[i]);
}
