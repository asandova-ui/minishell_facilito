/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:21:53 by marvin            #+#    #+#             */
/*   Updated: 2023/09/29 18:31:12 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"
#include "../../libft/libft.h"

static void	ft_revstr(char *str)
{
	int		size;
	char	tmp;
	char	*start;
	char	*end;

	size = ft_strlen(str);
	start = str;
	end = str + size - 1;
	while (start < end)
	{
		tmp = *start;
		*start = *end;
		*end = tmp;
		start++;
		end--;
	}
}

static int	num_len(unsigned int n)
{
	int	count;

	count = 1;
	while (n > 9)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_uitoa(unsigned int n)
{
	char	*str;
	size_t	length;

	str = (char *)ft_calloc(num_len(n) + 1, sizeof(char));
	if (!str)
		return (NULL);
	if (n == 0)
		str[0] = '0';
	length = 0;
	while (n != 0)
	{
		str[length++] = '0' + (n % 10);
		n /= 10;
	}
	ft_revstr(str);
	return (str);
}
