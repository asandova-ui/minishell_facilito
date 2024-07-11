/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 11:12:22 by asandova            #+#    #+#           */
/*   Updated: 2023/09/22 11:12:22 by asandova           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */

#include "./stdlib.h"

static int	count_words(const char *str, char c)
{
	int			flag;
	int			count;
	const char	*temp;

	temp = str;
	flag = 0;
	count = 0;
	while (*temp)
	{
		if (*temp == c && flag)
			flag = 0;
		if (*temp != c && !flag)
		{
			count++;
			flag++;
		}
		temp++;
	}
	return (count);
}

static int	custom_len(const char *s, char c)
{
	const char	*temp;
	int			len;

	temp = s;
	len = 0;
	while (*temp)
	{
		if (*temp == c)
			return (len);
		len++;
		temp++;
	}
	return (len);
}

static void	custom_cpy(char *dst, const char *src, int len)
{
	int	i;

	i = -1;
	while (++i < len && src[i])
		dst[i] = src[i];
	dst[i] = '\0';
}

static void	free_tab(char **tab, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		i;
	int		j;

	j = count_words(s, c);
	str = (char **)malloc(sizeof(char *) * (j + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (*s && j > 0)
	{
		if (!custom_len(s, c))
			s++;
		else
		{
			str[i++] = (char *)malloc(sizeof(char) * custom_len(s, c) + 1);
			if (!str[i - 1])
				return (free_tab(str, i), NULL);
			custom_cpy(str[i - 1], s, custom_len(s, c));
			s = s + custom_len(s, c);
			j--;
		}
	}
	str[i] = NULL;
	return (str);
}
