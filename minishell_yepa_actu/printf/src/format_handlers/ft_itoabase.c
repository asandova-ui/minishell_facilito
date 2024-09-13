/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoabase.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asandova <asandova@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 18:42:07 by marvin            #+#    #+#             */
/*   Updated: 2023/09/29 18:31:14 by asandova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"
#include "../../libft/libft.h"

static void	ft_revstr(char *str, int size)
{
	char	tmp;
	char	*start;
	char	*end;

	start = str;
	end = str + size;
	while (start < end)
	{
		tmp = *start;
		*start = *end;
		*end = tmp;
		start++;
		end--;
	}
}

static int	num_len(unsigned int n, unsigned int base_len)
{
	int	count;

	count = 1;
	while (n > base_len - 1)
	{
		n /= base_len;
		count++;
	}
	return (count);
}

char	*ft_itoa_base(unsigned int n, char *base)
{
	char			*str;
	unsigned int	length;
	int				base_len;

	base_len = (int)ft_strlen(base);
	str = ft_calloc(num_len(n, base_len) + 1, sizeof(char));
	if (!str)
		return (NULL);
	if (n == 0)
		str[0] = '0';
	length = 0;
	while (n != 0)
	{
		str[++length] = base[n % base_len];
		n = (n / base_len);
	}
	ft_revstr(str, length);
	return (str);
}
