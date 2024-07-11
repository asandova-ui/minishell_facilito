/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 11:04:53 by asandova            #+#    #+#           */
/*   Updated: 2023/09/22 11:04:53 by asandova           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

static int	is_sep(char c, const char *sep)
{
	size_t		i;
	const char	*aux;

	i = 0;
	aux = sep;
	while (aux[i])
	{
		if ((int)aux[i] == (int)c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char		*str;
	size_t		i;
	const char	*start;
	const char	*end;

	start = s1;
	end = start + ft_strlen(s1) - 1;
	while (*start && is_sep(*start, set))
		start++;
	while (end > start && is_sep(*end, set))
		end--;
	str = (char *)malloc(sizeof(char) * (end - start + 2));
	if (!str)
		return (NULL);
	i = 0;
	while (start <= end)
		str[i++] = *start++;
	str[i] = '\0';
	return (str);
}
