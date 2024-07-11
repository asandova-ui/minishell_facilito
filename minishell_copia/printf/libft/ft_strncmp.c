/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 17:57:08 by asandova            #+#    #+#           */
/*   Updated: 2023/09/18 17:57:08 by asandova           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	*str2;

	str = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (n--)
	{
		if (str[i] != str2[i] || str[i] == 0 || str2[i] == 0)
			return (str[i] - str2[i]);
		i++;
	}
	return (0);
}
