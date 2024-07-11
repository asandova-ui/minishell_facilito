/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 11:21:41 by asandova            #+#    #+#           */
/*   Updated: 2023/09/22 11:21:41 by asandova           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	char				*str;
	unsigned int		i;

	str = ft_strdup(s);
	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		str[i] = (*f)(i, str[i]);
		i++;
	}
	return (str);
}
